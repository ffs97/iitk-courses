package in.ac.iitk.cse.cs252.miners;

import java.text.MessageFormat;


public abstract class BaseMiner implements Miner {
    private int connectivity;
    private int hashRate;
    private final String id;

    protected BaseMiner(String id, int hashRate, int connectivity) {
        this.id = id;
        this.hashRate = hashRate;
        this.connectivity = connectivity;
    }

    @Override
    public final int getConnectivity() {
        return connectivity;
    }

    @Override
    public final void setConnectivity(int connectivity) {
        this.connectivity = connectivity;
    }

    @Override
    public final void setHashRate(int hashRate) {
        this.hashRate = hashRate;
    }

    @Override
    public final int getHashRate() {
        return hashRate;
    }

    @Override
    public String getId() {
        return id;
    }

    @Override
    public String toString() {
        return MessageFormat.format("Miner[id={0},hashRate={1},connectivity={2}]", id, hashRate, connectivity);
    }

}
