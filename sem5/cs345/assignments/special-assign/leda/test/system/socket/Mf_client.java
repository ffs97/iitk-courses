import java.io.*;

class Mf_client {
	public static void main (String[] args) throws IOException, UnsupportedEncodingException {
		if (args.length < 3){
			System.err.println ("usage: host port dimacs");
			System.exit(1);
		}
		String host = args[0]; 
		int port = Integer.parseInt(args[1]); 
		String	dimacs_name = args[2];

		File 	dimacs_f = new File (dimacs_name);
		if (!dimacs_f.exists()){
			abort ("Mf_client: no such dimacs file " + dimacs_name);
		}

		FileInputStream dimacs_s = null;
		StringBuffer str_b = null;
		try {
			dimacs_s = new  FileInputStream (dimacs_f);
			str_b = new StringBuffer();

			byte[] buffer = new byte[4096];
			int bytes_read;

			while ((bytes_read = dimacs_s.read(buffer)) != -1) {
				String s = new String(buffer, "US-ASCII");
        str_b.append(s.substring(1,bytes_read));
			}
				
		}
		finally {
			if (dimacs_s != null)		try {dimacs_s.close();}catch (IOException e) { 
						System.err.println("In finally catch");
				; }
		}

		LedaSocket sock = new LedaSocket(host, port);	
		LedaOutputStream out =sock.getLedaOutputStream();	
		LedaInputStream in =sock.getLedaInputStream();	

		out.writeLedaString(str_b.toString());
		String receive=in.readLedaString();

		System.out.println(receive);

	}

	private static void abort (String msg) throws IOException{
		throw new IOException(msg);	
	}
}
