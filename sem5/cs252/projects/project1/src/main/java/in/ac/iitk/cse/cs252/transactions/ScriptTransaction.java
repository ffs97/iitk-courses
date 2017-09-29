package in.ac.iitk.cse.cs252.transactions;

import com.google.common.collect.ImmutableSet;
import org.bitcoinj.core.*;
import org.bitcoinj.crypto.TransactionSignature;
import org.bitcoinj.kits.WalletAppKit;
import org.bitcoinj.params.MainNetParams;
import org.bitcoinj.script.Script;
import org.bitcoinj.script.ScriptBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.spongycastle.util.encoders.Hex;

import java.io.File;
import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * Created by bbuenz on 24.09.15.
 */
public abstract class ScriptTransaction implements AutoCloseable {

    private final WalletAppKit kit;
    private final NetworkParameters parameters;
    private final static Logger LOGGER = LoggerFactory.getLogger(ScriptTransaction.class);

    /**
     * @param parameters which network to use
     * @param file You wallet directory
     * @param password The password to secure your wallet
     */
    public ScriptTransaction(NetworkParameters parameters, File file, String password) {
        this.parameters = parameters;
        kit = new WalletAppKit(parameters, file, password);
        LOGGER.info("Starting to sync blockchain. This might take a few minutes");
        kit.setAutoSave(true);
        kit.startAsync();
        kit.awaitRunning();
        kit.wallet().allowSpendingUnconfirmedTransactions();
        LOGGER.info("Synced blockchain.");
        LOGGER.info("You've got {} in your pocket", kit.wallet().getBalance().toFriendlyString());
        try {
            kit.peerGroup().addAddress(InetAddress.getByName("eligius.st"));
            kit.peerGroup().addAddress(InetAddress.getByName("test-insight.bitpay.com"));
            kit.peerGroup().addAddress(InetAddress.getByName("insight.bitpay.com"));

        } catch (UnknownHostException ex) {
            LOGGER.warn("Failled to add eligius", ex);
        }
    }

    public Wallet getWallet() {
        return kit.wallet();
    }

    public abstract Script createInputScript();

    public abstract Script createRedemptionScript(Transaction unsignedTransaction);

    protected TransactionSignature sign(Transaction transaction, ECKey key) {
        Script script = transaction.getInput(0).getConnectedOutput().getScriptPubKey();
        return transaction.calculateSignature(0, key, script, Transaction.SigHash.ALL, false);
    }


    public void testScript(Script inputScript, Script spendingScript,Transaction redemptionTransaction) {
        spendingScript.correctlySpends(redemptionTransaction, 0, inputScript, ImmutableSet.of(Script.VerifyFlag.P2SH));
    }

    public Transaction createOutgoingTransaction(Script script, Coin amount) throws InsufficientMoneyException {
        Transaction transaction = new Transaction(parameters);
        transaction.addOutput(amount, script);
        Wallet.SendRequest request = Wallet.SendRequest.forTx(transaction);
        kit.wallet().completeTx(request);
        return transaction;
    }


    public Transaction createUnsignedRedemptionTransaction(TransactionOutput output, Address receiveAddress) throws InsufficientMoneyException {
        Transaction newTransaction = new Transaction(parameters);
        newTransaction.addOutput(output.getValue().subtract(Coin.MILLICOIN.multiply(2)), receiveAddress);
        newTransaction.addInput(output);
        return newTransaction;
    }

    public void sendTransaction(Transaction transaction) throws InsufficientMoneyException {
        Wallet.SendRequest request = Wallet.SendRequest.forTx(transaction);
        LOGGER.info("Transaction hex you can directly submit this to a block explorer:\n{}", new String(Hex.encode(transaction.bitcoinSerialize())));
        kit.wallet().commitTx(transaction);
        kit.peerGroup().broadcastTransaction(transaction);
        LOGGER.info("Broadcasted transaction: {}", transaction.getHashAsString());

    }

    /**
     * @return a random ECKey that hasn't yet been added to the wallet
     */
    public ECKey randKey() {
        return new ECKey();
    }

    public void importKey(ECKey key) {
        getWallet().importKey(key);
    }

    public Address getReceiveAddress() {
        return getWallet().currentReceiveAddress();
    }

    protected NetworkParameters getParameters() {
        return parameters;
    }

    @Override
    public void close() throws Exception {
        kit.stopAsync();
        kit.awaitTerminated();
    }
}
