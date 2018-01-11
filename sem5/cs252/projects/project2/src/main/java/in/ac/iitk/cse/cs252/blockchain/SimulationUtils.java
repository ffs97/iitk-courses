package in.ac.iitk.cse.cs252.blockchain;

import java.util.Random;


public final class SimulationUtils {
    private static final Random RNG = new Random(12345);

    private SimulationUtils() {
    }

    /**
     * @return a uniformly sampled value between 0 and 1
     */
    public static double sampleUniform() {
        return RNG.nextDouble();
    }

    /**
     * @param mu    mean of the normal
     * @param sigma sd of the normal
     * @return sample from N(mu,sigma^2)
     */
    public static double sampleNormal(double mu, double sigma) {
        return RNG.nextGaussian() * sigma + mu;
    }

    public static double sampleLogNormal() {
        return Math.exp( RNG.nextGaussian());
    }

    /**
     * Samples a random number from an exponential distribution with parameter lambda
     *
     * @param lambda The inverse of the mean of the distribution
     * @return a random sample drawn from Exp(lambda)
     */
    public static double sampleExponential(double lambda) {
        return Math.log(1 - RNG.nextDouble()) / -lambda;
    }
}
