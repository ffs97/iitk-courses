package in.ac.iitk.cse.cs252.miners;

import in.ac.iitk.cse.cs252.blockchain.Block;
import in.ac.iitk.cse.cs252.blockchain.NetworkStatistics;

public class SelfishMiner extends BaseMiner implements Miner {
    private Block publicChain;

    private Block privateChain;
//    private int flag;

    private int privateBlocklength;
//    private int minerHashRate, totalHashRate;


    public SelfishMiner(String id, int hashRate, int connectivity) {
        super(id, hashRate, connectivity);
//        this.minerHashRate = hashRate;

    }

    @Override
    public Block currentlyMiningAt() {
        return privateChain;

    }

    @Override
    public Block currentHead() {
        return publicChain;
    }

    @Override
    public void blockMined(Block block, boolean isMinerMe) {
        if(isMinerMe) {
            if (block.getHeight() > privateChain.getHeight()) {
                this.privateChain = block;
            }
        }
        else{
            if (block.getHeight() > privateChain.getHeight()) {
                privateChain = block;
                publicChain = block;
            } else if (block != null && (privateChain.getHeight()-block.getHeight()) <=1) {
                publicChain = privateChain;

            }

        }
    }


    @Override
    public void initialize(Block genesis, NetworkStatistics networkStatistics) {
        this.publicChain = genesis;
        this.privateChain=genesis;
        privateBlocklength = 0;
    }

    @Override
    public void networkUpdate(NetworkStatistics statistics) {

    }


}
