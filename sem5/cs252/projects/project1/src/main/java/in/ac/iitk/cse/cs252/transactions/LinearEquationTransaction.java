package in.ac.iitk.cse.cs252.transactions;

import org.bitcoinj.core.NetworkParameters;
import org.bitcoinj.core.Transaction;
import org.bitcoinj.core.Utils;
import org.bitcoinj.crypto.DeterministicKey;
import org.bitcoinj.crypto.TransactionSignature;
import org.bitcoinj.script.Script;
import org.bitcoinj.script.ScriptBuilder;

import java.io.File;
import java.math.BigInteger;
import java.net.UnknownHostException;

import static org.bitcoinj.script.ScriptOpCodes.*;

public class LinearEquationTransaction extends ScriptTransaction {
    private DeterministicKey key;

    public LinearEquationTransaction(NetworkParameters parameters, File file, String password) {
        super(parameters, file, password);
        key = getWallet().freshReceiveKey();
    }

    @Override
    public Script createInputScript() {
        // My roll number is 150259 => (150, 260)
        ScriptBuilder builder = new ScriptBuilder();

        builder.op(OP_2DUP);                                   // To duplicate top two entries in the stack i.e. x and y
        builder.op(OP_ADD);                                    // To add top two entries and push the result i.e x + y\

        builder.data(encode(new BigInteger("150")));       // To verify the equivalence of x + y = 150

        builder.op(OP_EQUALVERIFY);                            // To verify top two values are equal, else stop
        builder.op(OP_SUB);                                    // To subtract top two entries and push the result i.e x - y

        builder.data(encode(new BigInteger("260")));       // To verify the equivalence of x - y = 260

        builder.op(OP_EQUAL);                                  // To push TRUE to the stack if top two values are equal, else FALSE
        return builder.build();
    }

    @Override
    public Script createRedemptionScript(Transaction unsignedTransaction) {
        // Solution = (205, -55)
        TransactionSignature txSig = sign(unsignedTransaction, key);

        ScriptBuilder builder = new ScriptBuilder();
        builder.data(encode(new BigInteger("205")));        // x
        builder.data(encode(new BigInteger("-55")));        // y
        return builder.build();
    }

    private byte[] encode(BigInteger bigInteger) {
        return Utils.reverseBytes(Utils.encodeMPI(bigInteger, false));
    }
}
