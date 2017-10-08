import java.io.*;
/** 
 */
class LedaOutputStream extends FilterOutputStream{

	public LedaOutputStream (OutputStream in){ 
		super(in);
	}
	public void writeLedaString (String sout) throws IOException,
			UnsupportedEncodingException  {
		//System.out.println(sout.length());
		writeStringSize(sout.length());
		//writeStringSize (2);
		//System.out.println ("bytes: " + sout.getBytes("ASCII")[1]);
		//write (65); write (66);
		write(sout.getBytes("ASCII"));
		flush();
	}

	private void writeStringSize (int sz) throws IOException {
		for (int i = 0; i < 4; i++){
			
			write (sz%256);
			//System.out.println("sz out: " + sz%256);
			sz /= 256;
		}
	}
	private void error (String s){
		System.err.println(s);
		System.exit (1);
	} 

}
