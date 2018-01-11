package in.ac.iitk.cse.cs252.miners;

import com.google.common.collect.ImmutableList;
import in.ac.iitk.cse.cs252.blockchain.*;
import org.fest.assertions.Assertions;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Collection;
import java.util.Map;
import java.util.TreeMap;

/**
 * Created by bbuenz on 07.10.15.
 */
public class MiningSimulation {
    private final static Logger LOGGER = LoggerFactory.getLogger(MiningSimulation.class);

    @Test
    public void simulateCompliantMiners() {
        Miner miner1 = new CompliantMiner("Miner1", 51, 1);
        Miner miner2 = new CompliantMiner("Miner2", 15, 1);
        Miner miner3 = new CompliantMiner("Miner3", 14, 1);
        Miner miner4 = new CompliantMiner("Miner4", 10, 1);
        Miner miner5 = new CompliantMiner("Miner5", 5, 1);
        Miner miner6 = new CompliantMiner("Miner6", 5, 1);

        ImmutableList<Miner> miners = ImmutableList.of(miner1, miner2, miner3, miner4, miner5, miner6);
        runSimulation(miners, BlockReward.ONE, ChurnFunction.NO_CHURN);
    }

    @Test
    public void simulateDifferentNetworkPower() {
        Miner miner1 = new CompliantMiner("Miner1", 510, 10);
        Miner miner2 = new CompliantMiner("Miner2", 150, 5);
        Miner miner3 = new CompliantMiner("Miner3", 140, 1);
        Miner miner4 = new CompliantMiner("Miner4", 100, 3);
        Miner miner5 = new CompliantMiner("Miner5", 50, 1);
        Miner miner6 = new CompliantMiner("Miner6", 50, 1);

        ImmutableList<Miner> miners = ImmutableList.of(miner1, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(4567);
        ChurnFunction churn = new NormalChurnFunction(1, 1, rng);
        runSimulation(miners, BlockReward.ONE, churn);
    }


    //TODO: Uncomment this part when testing your miners.
    @Test
    public void simulate51PercentAttack1() {
        Miner attacker = new MajorityMiner("Attacker", 520, 1);
        Miner miner2 = new CompliantMiner("Miner2", 200, 1);
        Miner miner3 = new CompliantMiner("Miner3", 130, 1);
        Miner miner4 = new CompliantMiner("Miner4", 90, 1);
        Miner miner5 = new CompliantMiner("Miner5", 50, 1);
        Miner miner6 = new CompliantMiner("Miner6", 10, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(1234);
        ChurnFunction churn = new NormalChurnFunction(3, 1,rng);
        Map<String, Double> relativeProfits = runSimulation(miners, BlockReward.ONE, churn);
        Assertions.assertThat(relativeProfits.get(attacker.getId())).isGreaterThan(.55);
    }

    @Test
    public void simulate51PercentAttack2() {
        Miner attacker = new MajorityMiner("Attacker", 550, 1);
        Miner miner2 = new CompliantMiner("Miner2", 150, 1);
        Miner miner3 = new CompliantMiner("Miner3", 100, 1);
        Miner miner4 = new CompliantMiner("Miner4", 100, 1);
        Miner miner5 = new CompliantMiner("Miner5", 50, 1);
        Miner miner6 = new CompliantMiner("Miner6", 50, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(2345);
        ChurnFunction churn = new NormalChurnFunction(2, 3, rng);
        Map<String, Double> relativeProfits = runSimulation(miners, BlockReward.ONE, churn);
        Assertions.assertThat(relativeProfits.get(attacker.getId())).isGreaterThan(.6);
    }


    @Test
    public void simulateSelfishMining1() {
        Miner attacker = new SelfishMiner("Attacker", 34, 1);
        Miner miner2 = new CompliantMiner("Miner2", 15, 1);
        Miner miner3 = new CompliantMiner("Miner3", 10, 1);
        Miner miner4 = new CompliantMiner("Miner4", 15, 1);
        Miner miner5 = new CompliantMiner("Miner5", 10, 1);
        Miner miner6 = new CompliantMiner("Miner6", 10, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        Map<String, Double> relativeProfits = runSimulation(miners, BlockReward.ONE, ChurnFunction.NO_CHURN);
        double attackerProfits = relativeProfits.get(attacker.getId());
        Assertions.assertThat(attackerProfits).isGreaterThan(.375);
    }

    @Test
    public void simulateSelfishMining2() {
        Miner attacker = new SelfishMiner("Attacker", 270, 20);
        Miner miner2 = new CompliantMiner("Miner2", 150, 1);
        Miner miner3 = new CompliantMiner("Miner3", 100, 1);
        Miner miner4 = new CompliantMiner("Miner4", 150, 1);
        Miner miner5 = new CompliantMiner("Miner5", 100, 1);
        Miner miner6 = new CompliantMiner("Miner6", 100, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(3456);
        ChurnFunction churn = new NormalChurnFunction(1, 1,rng);
        Map<String, Double> relativeProfits = runSimulation(miners, BlockReward.ONE, churn);
        double attackerProfits = relativeProfits.get(attacker.getId());
        Assertions.assertThat(attackerProfits).isGreaterThan(.35);
    }


    @Test
    public void simulateFeeSniping1() {
        Miner attacker = new FeeSnipingMiner("Attacker", 300, 1);
        Miner miner2 = new CompliantMiner("Miner2", 150, 1);
        Miner miner3 = new CompliantMiner("Miner3", 150, 1);
        Miner miner4 = new CompliantMiner("Miner4", 200, 1);
        Miner miner5 = new CompliantMiner("Miner5", 100, 1);
        Miner miner6 = new CompliantMiner("Miner6", 100, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(1234);
        BlockReward reward = new LognormalReward(rng);
        Map<String, Double> relativeProfits = runSimulation(miners, reward, ChurnFunction.NO_CHURN);
        double attackerProfits = relativeProfits.get(attacker.getId());
        Assertions.assertThat(attackerProfits).isGreaterThan(.325);
    }

    @Test
    public void simulateFeeSniping2() {
        Miner attacker = new FeeSnipingMiner("Attacker", 290, 1);
        Miner miner2 = new CompliantMiner("Miner2", 220, 1);
        Miner miner3 = new CompliantMiner("Miner3", 190, 1);
        Miner miner4 = new CompliantMiner("Miner4", 150, 1);
        Miner miner5 = new CompliantMiner("Miner5", 130, 1);
        Miner miner6 = new CompliantMiner("Miner6", 20, 1);

        ImmutableList<Miner> miners = ImmutableList.of(attacker, miner2, miner3, miner4, miner5, miner6);
        SimulationRandom rng = new SimulationRandom(5678);
        BlockReward reward = new LognormalReward(rng);
        ChurnFunction churn = new NormalChurnFunction(0.2, 0.2, rng);
        Map<String, Double> relativeProfits = runSimulation(miners, reward, churn);
        double attackerProfits = relativeProfits.get(attacker.getId());
        Assertions.assertThat(attackerProfits).isGreaterThan(.31);
    }


    /**
     * Runs the simulation and returns a relative
     *
     * @param miners
     * @param rewardFunction
     * @param churnFunction
     * @return
     */
    private Map<String, Double> runSimulation(Collection<Miner> miners, BlockReward rewardFunction, ChurnFunction churnFunction) {
        int numIterations = 120;
        BitcoinNetwork networkControler = new BitcoinNetwork(rewardFunction, churnFunction, 0.005, 0.02d);
        Map<String, Double> profits = new TreeMap<>();
        SimulationRandom rng = new SimulationRandom(2345);
        for (int i = 0; i < numIterations; ++i) {
            int numBlocks = (int) rng.sampleExponentialRandom(0.0001);
            rewardFunction.reset();
            Block head = networkControler.simulation(numBlocks, miners, rng);
            Block current = head;
            while (current != null) {
                String winningMiner = current.getMinedBy();
                profits.merge(winningMiner, current.getBlockValue(), Double::sum);
                current = current.getPreviousBlock();
            }
        }
        Map<String, Double> relativeProfits = new TreeMap<>();
        double totalProfits = profits.values().stream().mapToDouble(Double::doubleValue).sum();
        for (String winingMiner : profits.keySet()) {
            double profit = profits.get(winingMiner);
            double relativeProfit = profit / totalProfits;
            LOGGER.info("{} has made {}% of the profits", winingMiner, 100d * relativeProfit);
            relativeProfits.put(winingMiner, relativeProfit);
        }
        return relativeProfits;
    }
}
