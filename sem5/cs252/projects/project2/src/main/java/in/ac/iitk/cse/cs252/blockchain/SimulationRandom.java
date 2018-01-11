package in.ac.iitk.cse.cs252.blockchain;

import java.util.Random;


public final class SimulationRandom extends Random {

    public SimulationRandom() {
        super();
    }

    public SimulationRandom(long seed) {
        super(seed);
    }


    /**
     * @param mu    mean of the normal
     * @param sigma sd of the normal
     * @return sample from N(mu,sigma^2)
     */
    public double sampleNormal(double mu, double sigma) {
        return nextGaussian() * sigma + mu;
    }

    public double sampleLogNormal(double mu,double sigma) {
        return Math.exp(mu+nextGaussian()*sigma);
    }

    /**
     * Samples a random number from an exponential distribution with parameter lambda
     *
     * @param lambda The inverse of the mean of the distribution
     * @return a random sample drawn from Exp(lambda)
     */
    public double sampleExponential(double lambda) {
        return Math.log(1 - nextDouble()) / -lambda;
    }
    public double sampleExponentialRandom(double lambda) {
        return Math.log(1 - new Random().nextDouble()) / -lambda;
    }
}
