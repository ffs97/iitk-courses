package in.ac.iitk.cse.cs252.blockchain;


public class NetworkStatistics {
    private final double orphanRate;
    private final int totalHashRate;
    private final int totalConnectivity;

    public NetworkStatistics(double orphanRate, int totalHashRate, int totalConnectivity) {
        this.orphanRate = orphanRate;
        this.totalHashRate = totalHashRate;
        this.totalConnectivity = totalConnectivity;
    }

    /**
     *
     * @return how likely is a block to orphan
     */
    public double getOrphanRate() {
        return orphanRate;
    }

    /**
     *
     * @return Total network connectivity value
     */
    public int getTotalConnectivity() {
        return totalConnectivity;
    }

    /**
     *
     * @return Total network hashrate
     */
    public int getTotalHashRate() {
        return totalHashRate;
    }
}
