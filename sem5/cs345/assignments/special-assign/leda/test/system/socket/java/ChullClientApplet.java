import java.awt.*;
import java.applet.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.border.*;
import java.io.*;
import java.util.*;
import java.net.*;

public class ChullClientApplet extends JApplet {
  JPanel contentPane = (JPanel) this.getContentPane();
  leda_socket socket = new leda_socket();

  Vector points = new Vector();  
  Vector result = new Vector();
  
  ButtonGroup operation = new ButtonGroup();
  MyCanvas readArea;
  Graphics gr;

  public void processMouseEvent(MouseEvent me) {
      gr.drawRect(me.getX(), me.getY(), 10, 10);
  }

  class MyCanvas extends Canvas {
  
    int mousex = 0, mousey = 0;
    int click = 0;

    class MyMouseAdapter extends MouseAdapter {

      public void mousePressed(MouseEvent me) {
        if (click == 2) {
          if (me.getModifiers() == MouseEvent.BUTTON1_MASK) {
            points.addElement(me.getPoint());
            repaint();
          } else if (me.getModifiers() == MouseEvent.BUTTON3_MASK) {
            click = 1;
 
            repaint();
          }
        } else if (me.getModifiers() == MouseEvent.BUTTON1_MASK) {
                 if (click == 0) {
  
                 }
                 click = 2;
                 mousex = me.getX();
                 mousey = me.getY();
                 points.addElement(me.getPoint());
                 repaint();
               } else if (me.getModifiers() == MouseEvent.BUTTON3_MASK) {
                 click = 0;
   
                 repaint();
               }

      }
    }

    class MyMouseMotionAdapter extends MouseMotionAdapter {

      public void mouseMoved(MouseEvent me) {
        if (click == 2 && !points.isEmpty()) {
          mousex = me.getX();
          mousey = me.getY();
          repaint();
        }
      }
    }

    public MyCanvas() {
      addMouseListener(new MyMouseAdapter());
      addMouseMotionListener(new MyMouseMotionAdapter());
      setBackground(Color.white);
    }

    public void paint(Graphics g) {

      g.setColor(Color.black);
      for (int i = 0; i < points.size(); i++) {
        Point pact = (Point) points.get(i);
        g.drawLine(pact.x, pact.y, pact.x +1, pact.y +1);
      }
      
      //draw convex hull ...
      for (int i = 1; i < result.size(); i++) {
        Point u = (Point) result.get(i-1);
        Point v = (Point) result.get(i);
        g.drawLine(u.x, u.y, v.x, v.y);
      }
      if (!result.isEmpty()) {
          Point u = (Point) result.lastElement();
          Point v = (Point) result.firstElement();
          g.drawLine(u.x, u.y, v.x, v.y);
      }
                  
      g.setColor(Color.blue);

    }
  } // end of class MyCanvas

  class BtnListener implements ActionListener {
    public Point get_point(String s)
    {
      //remove "head"
      int l = s.length()-3;
      int x=0,y=0;
      
      try {
        // remove head and tail ...
        String s2 = s.substring(1,l);
	//System.out.println(s2);
	
	// get x and y coordinates ...
	int index = s2.indexOf(",");
	
	String sub1 = s2.substring(0,index);
	String sub2 = s2.substring(index+1);
	
	System.out.println(sub1+"  "+sub2);
	
	x = Integer.parseInt(sub1);
	y = Integer.parseInt(sub2);
      }
      catch(Exception e) { }
      
      Point p = new Point(x,y);
      return p;
    }
  

    public void actionPerformed(ActionEvent ae) {
      String ac = ae.getActionCommand();
      if (ac == "new") {
        points.removeAllElements();
	result.removeAllElements();
        readArea.repaint();
      }
      if (ac == "send") {
          socket.connect();

          // send points
	  send();
          int i,numb;
	  result.removeAllElements();
	  
	  try {
	   String recv = socket.receive_string();
	   numb = Integer.parseInt(recv);
	  
	   for(i=0;i<numb;i++){	  
            recv = socket.receive_string();
	    result.addElement(get_point(recv));
           }
          }
	  catch(NumberFormatException e){ }

          socket.disconnect();
          //readArea.paint();

          readArea.repaint();
      }
    }

  } // end of class BtnListener

  public void init() {
    socket.set_host("localhost");
    socket.set_port(9999);
    setSize(500, 500);
    contentPane.setLayout(new BorderLayout());

    BtnListener btnLstnr = new BtnListener();
    JButton readBtn = new JButton("new");
    readBtn.setActionCommand("new");
    readBtn.addActionListener(btnLstnr);
    JButton sendBtn = new JButton("send");
    sendBtn.setActionCommand("send");
    sendBtn.addActionListener(btnLstnr);

    JPanel cmdPnl = new JPanel();
    cmdPnl.setBorder(new TitledBorder(" Command "));
    cmdPnl.setLayout(new GridLayout(0, 1));
    cmdPnl.add(readBtn);
    cmdPnl.add(sendBtn);

    JPanel cntrlPnl = new JPanel();
    cntrlPnl.add(cmdPnl);
    contentPane.add(cntrlPnl, BorderLayout.NORTH);

    readArea = new MyCanvas();
    contentPane.add(readArea, BorderLayout.CENTER);
    setVisible(true);
  }

  public void send() {
    //String s = a.toString() + "#" + operation.getSelection().getActionCommand()
    //           + "#" + b.toString();
    String s = new String(Integer.toString(points.size()));
    
    socket.send_string(s);
    
    for (int i = 0; i < points.size(); i++) {
        Point pact = (Point) points.get(i);
        s = "(" + pact.x + "," + pact.y + ",1)";
	socket.send_string(s);
    }    
    
    //System.out.println(s);   
    //socket.send_string(s);
  }

}