// Alexander Dann/ Christian Hoff (Uni Trier)

import java.io.*;
import java.net.*;

public class leda_socket {

  private String host = null;
  private int port = -1;
  private int queueLength = 50;
  private int timeout = 0;

  private Socket clientSocket = null;
  private ServerSocket serverSocket = null;
  private InputStreamReader in = null;
  private PrintWriter out = null;


  // constructor
  public leda_socket() {}
  public leda_socket(String host) { this.host = host; }
  public leda_socket(String host, int port) { this.host = host; this.port = port; }


  // methods
  void set_host(String host) { this.host = host; }
  void set_port(int port) { this.port = port; }
  void set_qlength(int len) { queueLength = len; }
  void set_timeout(int sec) {
    timeout = sec;
    try {
      if (clientSocket != null) clientSocket.setSoTimeout(1000 * timeout);
    } catch (SocketException se) {
      se.printStackTrace(System.out);
    }
  }
  String get_host() { return host; }
  int get_port() { return port; }
  int get_qlength() { return queueLength; }
  int get_timeout() { return timeout; }
  String get_ip() {
    try {
      return InetAddress.getByName(host).getHostAddress();
    } catch (UnknownHostException uhe) {
      System.err.println("Could not get IP for host: " + host);
      return null;
    }
  }

  boolean listen() {
    if (port == -1) System.err.println("error: no port");

    try {
      serverSocket = new ServerSocket(port);
    } catch (IOException ioe) {
      System.err.println("Could not listen on port: " + port);
      return false;
    }
    return true;
  } // end of listen()

  boolean accept() {
    if (serverSocket == null) {
      System.err.println("error: no socket endpoint on the server -> first use listen()");
      return false;
    }

    try {
      clientSocket = serverSocket.accept();
      clientSocket.setSoTimeout(1000 * timeout);
      out = new PrintWriter(clientSocket.getOutputStream(), false);
      in = new InputStreamReader(clientSocket.getInputStream());
    } catch (IOException ioe) {
      System.err.println("Accept failed.");
      return false;
    }
    return true;
  } // end of accept()

  boolean connect() {
    if (host == null) System.err.println("error: no host");
    if (port == -1) System.err.println("error: no port");

    try {
      clientSocket = new Socket(host, port);
      clientSocket.setSoTimeout(1000 * timeout);
      out = new PrintWriter(clientSocket.getOutputStream(), false);
      in = new InputStreamReader(clientSocket.getInputStream());
    } catch (UnknownHostException uhe) {
      System.err.println("Don't know about host: " + host);
      return false;
    } catch (IOException ioe)  {
      System.err.println("Couldn't get I/O for the connection to: " + host);
      return false;
    }
    return true;
  } // end of connect()

  void disconnect() {
    try {
      if (out != null) out.close();
      if (in != null) in.close();
      if (clientSocket != null) clientSocket.close();
      //if (serverSocket != null) serverSocket.close();
    } catch (IOException ioe) {
      System.err.println("I/O error while disconnect from: " + host);
    }
  } // end of disconnect()

  String receive_string() {
    StringBuffer sb = new StringBuffer();
    try {
      int length = in.read() + (in.read()<<8) + (in.read()<<16) + (in.read()<<24);
      for (int i = 0; i < length; sb.append((char) in.read()), i++);
    } catch (InterruptedIOException iioe) {
      System.err.println("timeout");
    } catch (IOException ioe) {
      System.err.println("I/O error while receiving from: " + host);
      return null;
    }
    return sb.toString();
  } // end of receive_string(String recv)

  boolean wait(String s) {
    return s.equals(receive_string());
  } // end of wait(String s)

  void send_string(String msg) {
    int l = msg.length();
    for (int i = 0; i < 4; i++) {
      out.write(l % 256);
      l /= 256;
    }
    out.print(msg);
    out.flush();
  } // end of send_string(String msg)

} // end of class leda_socket
