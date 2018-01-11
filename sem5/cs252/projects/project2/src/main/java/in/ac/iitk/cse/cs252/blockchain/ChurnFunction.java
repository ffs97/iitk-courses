package in.ac.iitk.cse.cs252.blockchain;

import in.ac.iitk.cse.cs252.miners.Miner;

import java.util.Collection;


public interface ChurnFunction {
    static ChurnFunction NO_CHURN = (orphanRate, miners) -> {
        int totalHashRate = miners.stream().mapToInt(Miner::getHashRate).sum();
        int totalConnectivity = miners.stream().mapToInt(Miner::getConnectivity).sum();
        return new NetworkStatistics(orphanRate, totalHashRate, totalConnectivity);
    };

    NetworkStatistics churnNetwork(double orphanRate, Collection<Miner> miners);


}
