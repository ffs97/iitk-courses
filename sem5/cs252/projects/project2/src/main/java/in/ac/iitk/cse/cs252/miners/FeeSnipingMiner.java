package in.ac.iitk.cse.cs252.miners;

import in.ac.iitk.cse.cs252.blockchain.Block;
import in.ac.iitk.cse.cs252.blockchain.NetworkStatistics;


public class FeeSnipingMiner extends BaseMiner implements Miner {
    private Block currentHead;
    private int unusualValue=35;



    public FeeSnipingMiner(String id, int hashRate, int connectivity) {
        super(id, hashRate, connectivity);
//        this.minerHashRate = hashRate;

    }

    @Override
    public Block currentlyMiningAt() {
        return currentHead;

    }

    @Override
    public Block currentHead() {
        return currentHead;
    }

    @Override
    public void blockMined(Block block, boolean isMinerMe) {
        if(isMinerMe) {
            if (block.getHeight() > currentHead.getHeight()) {
                this.currentHead = block;
            }
        }
        else{
            if(block.getBlockValue() < unusualValue){
                if (currentHead == null) {
                    currentHead = block;
                } else if (block != null && block.getHeight() > currentHead.getHeight()) {
                    this.currentHead = block;

                }
            }
        }
    }


    @Override
    public void initialize(Block genesis, NetworkStatistics networkStatistics) {
        this.currentHead = genesis;

    }

    @Override
    public void networkUpdate(NetworkStatistics statistics) {

    }
}
