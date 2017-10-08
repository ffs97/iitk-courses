import java.io.*;
import java.util.*;
/** 
*/
class LedaInputStream extends FilterInputStream{
	
	public LedaInputStream (InputStream in){ 
		super(in);
	}
	public String readLedaString() throws IOException{
		int sz = readStringSize();
		System.out.println ("Size in: " + sz);
		int  n = 0;
		int b;
		StringBuffer sb = new StringBuffer();
		while ((n < sz ) && (b = read()) != -1){
			//System.out.println ("n: " + n + "  b: " + b );   
			n ++ ;
			
			//for(int i=0;i<buf_fill;i++) sb.append(toChar(buf[i]));
			sb.append((char)b);
		}
		//System.out.println ("n: " + n );
		if (n != sz)error ("1:LServer has closed connection");
		return sb.toString ();
		
	}
	public static Vector toVector(String s){
		Vector v = new Vector();
		for (Enumeration e = new StringTokenizer (s, "\n\r");e.hasMoreElements();){
			v.addElement(e.nextElement());	
		}
		return v;
	}
	public void wait (String s)throws IOException{
		String s1 = readLedaString();
		/*if (s1.equals("agd_error")){
			System.out.println("error: " + readLedaString());
			System.exit(1);
		}*/
		if (!s1.equals(s))error ("got " + s1 + " instead of " + s + "!");
	}	
	private int readStringSize()throws IOException{
		//byte[] buf = new byte[4];
		//int err = read (buf);
		//if (err != 4)error("2:LServer has closed connection");
		int sz = 0;
		int factor = 1;
		for (int i=0; i < 4; i++){
			//int b = buf[i] < 0 ? -buf[i]+128 : buf[i];
			//int b = buf[i] << 8 >> 8;
			int b;
			b = read();
			if (b == -1)error("2:LServer has closed connection");
			//System.out.println ("buf["+ i +"]=" + b);
			sz +=b * factor;
			factor *= 256;
		}
		//return (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
		return sz;
	}
	private void error (String s){
		System.err.println(s);
		System.exit (1);
	} 
	
}
