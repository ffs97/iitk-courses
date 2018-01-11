package in.ac.iitk.cse.cs252.blockchain;

import javax.annotation.Nullable;

import java.text.MessageFormat;


public class Block {
    private final Block previousBlock;
    private final int height;
    private final String minedBy;
    private final double blockValue;


    Block(@Nullable Block previousBlock, String minedBy, double blockValue) {
        this.previousBlock = previousBlock;
        this.height = previousBlock == null ? 0 : previousBlock.height + 1;
        this.minedBy = minedBy;
        this.blockValue = blockValue;
    }

    /**
     * The Block that is extended by this.
     *
     * @return Block the previous Block
     */
    @Nullable
    public Block getPreviousBlock() {
        return previousBlock;
    }

    /**
     *
     * @return  The current height of the chain.
     */
    public int getHeight() {
        return height;
    }

    @Override
    public boolean equals(Object obj) {
        return super.equals(obj);
    }

    /**
     *
     * @return Sum of block reward and mining fees
     */
    public double getBlockValue() {
        return blockValue;
    }

    /**
     *
     * @return The id of the miner that mined this Block
     */
    public String getMinedBy() {
        return minedBy;
    }


    @Override
    public String toString() {
        return MessageFormat.format("Block[height={0},blockValue={1},minedBy={2}]", height, blockValue, minedBy);
    }
}
