package in.ac.iitk.cse.cs252.blockchain;

import in.ac.iitk.cse.cs252.miners.Miner;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.*;
import java.util.stream.Collectors;

import static java.util.AbstractMap.SimpleEntry;
import static java.util.Map.Entry;

public class BitcoinNetwork {
    private final BlockReward blockReward;
    private final double churnProbability;
    private final ChurnFunction churnFunction;
    private final double orphanRate;
    private static final Logger LOGGER = LoggerFactory.getLogger(BitcoinNetwork.class);
    private int forkStart = -1;
    private final SimulationRandom broadcastRNG;

    public BitcoinNetwork(BlockReward blockReward, ChurnFunction churnFunction, double churnProbability, double orphanRate) {
        this.blockReward = blockReward;
        this.churnProbability = churnProbability;
        this.churnFunction = churnFunction;
        this.orphanRate = orphanRate;
        this.broadcastRNG=new SimulationRandom();
    }

    public Block simulation(int numBlocks, Collection<Miner> miners) {
        return simulation(numBlocks, miners, new SimulationRandom());
    }

    /**
     * Runs a simulation of the Blockchain over numBlocks blocks
     *
     * @param numBlocks
     * @return Block the head of the longest chain
     */
    public Block simulation(int numBlocks, Collection<Miner> miners, SimulationRandom miningRNG) {
        //Genesis block
        forkStart = -1;
        Block genesis = new Block(null, "Satoshi", 0);
        int totalHashRate = miners.stream().mapToInt(Miner::getHashRate).sum();
        int totalConnectivity = miners.stream().mapToInt(Miner::getConnectivity).sum();
        Map<Miner,Double> minedRewards=new HashMap<>();

        NetworkStatistics statistics = new NetworkStatistics(orphanRate, totalHashRate, totalConnectivity);
        miners.forEach(m -> m.initialize(genesis, statistics));
        double singleOrphanRate = orphanRate / (orphanRate + 1);
        for (int i = 0; i < numBlocks; ++i) {
            if (miningRNG.nextDouble() < churnProbability) {
                NetworkStatistics networkStatistics = churnFunction.churnNetwork(orphanRate, miners);
                miners.forEach(m -> m.networkUpdate(networkStatistics));
            }
            List<Entry<Miner, Double>> winnningMiners = miners.stream()
                    .map(m -> new SimpleEntry<>(m, miningRNG.sampleExponential(m.getHashRate())))
                    .sorted(Comparator.comparing(Entry::getValue))
                    .collect(Collectors.toList());
            Map<BlockMessage, Double> initialMessages = new HashMap<>(miners.size());
            double currentOrphanProbability = singleOrphanRate;
            for (Entry<Miner, Double> winner : winnningMiners) {
                Miner winningMiner = winner.getKey();
                Block previousBlock = winningMiner.currentlyMiningAt();
                double reward = blockReward.computeBlockReward(previousBlock.getHeight() + 1, winner.getValue());
                Block nextBlock = new Block(previousBlock, winningMiner.getId(), reward);
                BlockMessage blockMessage = new BlockMessage(nextBlock, winningMiner);
                minedRewards.merge(winningMiner,reward,Double::sum);
                initialMessages.put(blockMessage, 0d);
                if (miningRNG.nextDouble() < currentOrphanProbability) {
                    currentOrphanProbability *= singleOrphanRate;
                } else {
                    break;
                }
            }
            propagateBlock(initialMessages, miners);

        }
        double totalRewards=minedRewards.values().stream().mapToDouble(Double::doubleValue).sum();
        minedRewards.replaceAll((m,d)->Math.round(1000*d/totalRewards)/10d);
        LOGGER.debug("Simulation finished with block mining distribution {}",minedRewards);
        return miners.stream().map(Miner::currentHead).max(Comparator.comparing(Block::getHeight)).get();
    }


    private void propagateBlock(Map<BlockMessage, Double> initialMessages, Collection<Miner> miners) {

        Set<BlockMessage> deliveredMessages = new HashSet<>(miners.size());

        Map<BlockMessage, Double> messageQueue = initialMessages;

        int minHeight = 0;

        while (!messageQueue.isEmpty()) {
            BlockMessage message = messageQueue.entrySet().stream().min(Entry.comparingByValue()).get().getKey();
            message.deliver();
            deliveredMessages.add(message);
            double currentTime = messageQueue.remove(message);
            Miner sender = message.recipient;
            Block broadcastBlock = sender.currentHead();
            if (broadcastBlock.getHeight() >= minHeight) {
                minHeight = Math.max(broadcastBlock.getHeight(), minHeight);
                miners.stream()
                        .map(m -> new BlockMessage(broadcastBlock, m))
                        .filter(m -> !deliveredMessages.contains(m))
                        .forEach(m -> messageQueue.putIfAbsent(m, currentTime + broadcastRNG.sampleExponential(m.recipient.getConnectivity() * sender.getConnectivity())));
            }


        }
        printForks(miners);

    }

    private void printForks(Collection<Miner> miners) {
        Map<Block, Integer> forks = miners.stream().collect(Collectors.toMap(Miner::currentHead, Miner::getHashRate, Integer::sum));
        if (forks.size() > 1 && forkStart < 0) {
            forkStart = forks.keySet().stream().mapToInt(Block::getHeight).max().getAsInt();
            LOGGER.debug("{} heads: {}", forks.size(), forks);
        } else {
            if (forkStart >= 0) {
                Block next = forks.keySet().iterator().next();
                LOGGER.debug("Fork resolved with winner being {} after {} blocks", next, next.getHeight() - forkStart);
                if (next.getHeight() - forkStart > 1) {
                    LOGGER.debug("Fork resolved with winner being {} after {} blocks", next, next.getHeight() - forkStart);
                }
                forkStart = -1;
            }
        }
    }

    private class BlockMessage {
        private final Block block;
        private final Miner recipient;

        private BlockMessage(Block block, Miner recipient) {
            this.block = block;
            this.recipient = recipient;
        }

        public void deliver() {
            if (block.getMinedBy().equals(recipient.getId())) {
                if (block.getHeight() >= recipient.currentlyMiningAt().getHeight()) {
                    recipient.blockMined(block, true);
                }
            } else {
                recipient.blockMined(block, false);
            }
        }

        @Override
        public boolean equals(Object other) {
            BlockMessage otherMessage = (BlockMessage) other;
            return block.equals(otherMessage.block) && recipient.equals(otherMessage.recipient);
        }

        @Override
        public int hashCode() {
            return Objects.hash(block, recipient);
        }
    }


}
