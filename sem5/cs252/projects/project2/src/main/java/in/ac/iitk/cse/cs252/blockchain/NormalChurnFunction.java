package in.ac.iitk.cse.cs252.blockchain;

import in.ac.iitk.cse.cs252.miners.Miner;

import java.util.Collection;


public class NormalChurnFunction implements ChurnFunction {
    private final double connectivitySD;
    private final double hashRateSD;
    private final SimulationRandom rng;

    public NormalChurnFunction(double hashRateSD, double connectivitySD) {
        this(hashRateSD, connectivitySD, new SimulationRandom());
    }

    public NormalChurnFunction(double hashRateSD, double connectivitySD, SimulationRandom rng) {
        this.hashRateSD = hashRateSD;
        this.connectivitySD = connectivitySD;
        this.rng = rng;
    }

    @Override
    public NetworkStatistics churnNetwork(double orphanRate, Collection<Miner> miners) {
        int totalHashRate = 0;
        int totalConnectivity = 0;
        for (Miner miner : miners) {
            int newConnectivity;
            do {
                newConnectivity = (int) Math.round(rng.sampleNormal(miner.getConnectivity(), connectivitySD));
            } while (newConnectivity < 1);
            totalConnectivity += newConnectivity;
            int newHashRate;
            do {
                newHashRate = (int) Math.round(rng.sampleNormal(miner.getHashRate(), hashRateSD));
            } while (newHashRate < 1);
            totalHashRate += newHashRate;

            miner.setHashRate(newHashRate);
            miner.setConnectivity(newConnectivity);
        }
        return new NetworkStatistics(orphanRate, totalHashRate, totalConnectivity);
    }

}
