package in.ac.iitk.cse.cs252.transactions;

import org.bitcoinj.core.ECKey;
import org.bitcoinj.core.NetworkParameters;
import org.bitcoinj.core.Transaction;
import org.bitcoinj.core.Utils;
import org.bitcoinj.crypto.DeterministicKey;
import org.bitcoinj.crypto.TransactionSignature;
import org.bitcoinj.script.Script;
import org.bitcoinj.script.ScriptBuilder;

import java.io.File;
import java.math.BigInteger;

import static org.bitcoinj.script.ScriptOpCodes.*;


public class MultiSigTransaction extends ScriptTransaction {
    private DeterministicKey keyBank;
    private DeterministicKey keyCustomer1;
    private DeterministicKey keyCustomer2;
    private DeterministicKey keyCustomer3;


    public MultiSigTransaction(NetworkParameters parameters, File file, String password) {
        super(parameters, file, password);

        keyBank = getWallet().freshReceiveKey();
        keyCustomer1 = getWallet().freshReceiveKey();
        keyCustomer2 = getWallet().freshReceiveKey();
        keyCustomer3 = getWallet().freshReceiveKey();
    }


    @Override
    public Script createInputScript() {
        // Assumes the signature is in any order with one bank and one customer (in case valid)
        ScriptBuilder builder = new ScriptBuilder();

        builder.op(OP_DUP);                                         // To duplicate the top value in the stack
        builder.data(keyBank.getPubKey());                          // To push bank's public key to the stack
        builder.op(OP_CHECKSIG);                                    // To check if the second signature is of bank

        builder.op(OP_IF);                                          // If second signature is by bank

        builder.op(OP_DROP);                                        // To delete the top entry i.e. the second signature

        builder.op(OP_1);                                           // Number of signatures to compare i.e. the first signature

        builder.data(keyCustomer1.getPubKey());                     // To verify if signature is of customer 1
        builder.data(keyCustomer2.getPubKey());                     // To verify if signature is of customer 2
        builder.data(keyCustomer3.getPubKey());                     // To verify if signature is of customer 3

        builder.op(OP_3);                                           // Number of signatures to be compared with

        builder.op(OP_CHECKMULTISIG);                               // To check MultiSig

        builder.op(OP_ELSE);                                        // If the second signature is not of bank

        builder.op(OP_SWAP);                                        // Swap the top two values in the stack
        builder.data(keyBank.getPubKey());                          // To verify the signature of bank
        builder.op(OP_CHECKSIGVERIFY);                              // Compare the top signature with bank's public key

        builder.op(OP_1);                                           // Number of signatures to compare i.e. the second signature

        builder.data(keyCustomer1.getPubKey());                     // To verify if signature is of customer 1
        builder.data(keyCustomer2.getPubKey());                     // To verify if signature is of customer 2
        builder.data(keyCustomer3.getPubKey());                     // To verify if signature is of customer 3

        builder.op(OP_3);                                           // Number of signatures to be compared with

        builder.op(OP_CHECKMULTISIG);                               // To check MultiSig

        builder.op(OP_ENDIF);

        return builder.build();
    }


    @Override
    public Script createRedemptionScript(Transaction unsignedTransaction) {
        TransactionSignature txSig1 = sign(unsignedTransaction, keyBank);
        TransactionSignature txSig2 = sign(unsignedTransaction, keyCustomer1);

        ScriptBuilder builder = new ScriptBuilder();

        builder.op(OP_1);                                              // Required Since CheckMultiSig removes an extra value from the stack (Can also be added in the input script)
        // OP_0 is not working, why??

        builder.data(txSig1.encodeToBitcoin());
        builder.data(txSig2.encodeToBitcoin());

        return builder.build();
    }
}
