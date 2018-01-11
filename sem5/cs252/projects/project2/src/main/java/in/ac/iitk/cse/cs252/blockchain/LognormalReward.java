package in.ac.iitk.cse.cs252.blockchain;

import java.util.ArrayList;
import java.util.List;

public class LognormalReward implements BlockReward {
    private final SimulationRandom rng;
    private final List<Double> previousRewards = new ArrayList<>();

    public LognormalReward(SimulationRandom rng) {
        this.rng = rng;
    }

    public LognormalReward() {
        this(new SimulationRandom());
    }

    @Override
    public double computeBlockReward(int height, double timeToCreate) {
        while (previousRewards.size() <= height) {

            previousRewards.add(rng.sampleLogNormal(0,2));
        }
        return previousRewards.get(height);

    }

    @Override
    public void reset() {
        previousRewards.clear();
    }
}
