
/*
 *  RoombaComm Interface Test
 *
 *  Copyright (c) 2005 Tod E. Kurt, tod@todbot.com, ThingM
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *  Boston, MA  02111-1307  USA
 *
 */


package roombacomm;

import java.util.*;

/**
  A fairly thorough test of the RoombaComm API.
  <p>
  Run it with something like: <pre>
    java roombacomm.Test /dev/cu.KeySerial1
  Usage:
  roombacomm.Test serialportname [protocol] [options]<br>
  where: protocol (optional) is SCI or OI
  [options] can be one or more of:
   -debug       -- turn on debug output
   -hwhandshake -- use hardware-handshaking, for Windows Bluetooth
   -nohwhandshake -- don't use hardware-handshaking
   -flush -- flush on sends(), normally not needed
   -power       -- power on/off Roomba (if interface supports it)
 * </pre>
 * </p>
 */
public class Robomain extends Thread {

    static String usage =
        "Usage: \n"+
        "  roombacomm.Test serialportname [protocol] [options]\n" +
        "where: protocol (optional) is SCI or OI\n" +
        "[options] can be one or more of:\n"+
        " -debug       -- turn on debug output\n"+
        " -hwhandshake -- use hardware-handshaking, for Windows Bluetooth\n"+
        " -nohwhandshake -- don't use hardware-handshaking\n"+
        " -flush       -- flush on sends(), normally not needed\n"+
        " -power       -- power on/off Roomba (if interface supports it)\n"+
        "\n";

    static boolean debug = false;
    static boolean hwhandshake = false;
    static boolean power = false;
    static boolean flush = false;
    static int locstatus = -1;
    RoombaCommSerial roombacomm = null;
    static boolean isStraight = false;
    static long millisecs = 0;
    static final int SPEED = 250;
    static int distance = 0;
    static int remdistance = 0;
    boolean pleaseWait = false;

    public Robomain() { }

    public Robomain(RoombaCommSerial roomba) {
    	roombacomm = roomba;
    }

  public static void main(String[] args) {

    if( args.length == 0 ) {
            System.out.println(usage);
            System.exit(0);
    }

    String portname = args[0];  // e.g. "/dev/cu.KeySerial1"
    RoombaCommSerial roombaComm = new RoombaCommSerial();

        for( int i=1; i < args.length; i++ ) {
        	if (args[i].equals("SCI") || (args[i].equals("OI"))) {
        		roombaComm.setProtocol(args[i]);
        	} else if( args[i].endsWith("debug") )
                debug = true;
            else if( args[i].endsWith("power") )
                power = true;
            else if( args[i].endsWith("nohwhandshake") )
                roombaComm.setWaitForDSR(false);
            else if( args[i].endsWith("hwhandshake") )
                roombaComm.setWaitForDSR(true);
            else if( args[i].endsWith("flush") )
                flush = true;
        }

    roombaComm.debug = debug;
    roombaComm.flushOutput = flush;

    String portlist[] = roombaComm.listPorts();
    System.out.println("Available ports:");
    for( int i=0; i < portlist.length; i++ )
       System.out.println("  "+i+": "+portlist[i]);

    if( ! roombaComm.connect( portname ) ) {
       System.out.println("Couldn't connect to "+portname);
       System.exit(1);
    }

    if( power ) {
       System.out.println("waking up Roomba...");
       roombaComm.wakeup();
    }

    System.out.println("Roomba startup");
    roombaComm.startup();
    roombaComm.control();
    roombaComm.pause(30);

    System.out.println("Checking for Roomba... ");
    if( roombaComm.updateSensors() ) {
       System.out.println("Roomba found!");
    }
    else {
       System.out.println("No Roomba. :(");
       System.exit(0);
    }

    // must pause after every playNote to let to note sound
    System.out.println("Playing some notes");
    roombaComm.playNote( 72, 10 );
    roombaComm.pause( 200 );
    roombaComm.playNote( 79, 10 );
    roombaComm.pause( 200 );
    roombaComm.playNote( 76, 10 );
    roombaComm.pause( 200 );

    // test Logo-like functions (blocking)
    // speed is in mm/s, go* is in mm, spin is in degrees

    // Create and start the thread
    Thread thread = new Robomain(roombaComm);
    thread.start();

    if( power ) {
      System.out.println("Powering off in 3 seconds.");
      roombaComm.pause( 3000 );
      roombaComm.powerOff();
    }

//    int distance = 0;
//    int bkdistance = 0;
    boolean robotadjusted = false;
    roombaComm.updateSensors();

    while(locstatus != 100) {

     robotadjusted = false;
     while(locstatus != 7 && (roombaComm.bumpLeft() || roombaComm.bumpRight())) {

       if(!robotadjusted) {
         setRemainDistance(roombaComm);

         synchronized (thread) { ((Robomain)thread).pleaseWait = true; }
         robotadjusted = true;
       }

         System.out.println("Handling = " + locstatus);
       //try {
        if(roombaComm.bumpLeft() && !roombaComm.bumpRight() && locstatus != 3 && locstatus != 4 && locstatus != 5) {
          System.out.println("Handling Left bump....");
      	  //thread.sleep(1000);
  	      roombaComm.goBackward( 15 );
	      roombaComm.spinRight(15);
  	      roombaComm.goForward(10);
        }
        else if(roombaComm.bumpLeft() && !roombaComm.bumpRight() && (locstatus == 3 || locstatus == 4 || locstatus == 5)) {
          System.out.println("Handling Left bump....");
      	  //thread.sleep(1000);
  	      roombaComm.goBackward( 15 );
	      roombaComm.spinLeft(15);
  	      roombaComm.goForward(10);
        }
        else if(roombaComm.bumpRight() && !roombaComm.bumpLeft()) {
          System.out.println("Handling Right bump....");
      	  //thread.sleep(1000);
  	      roombaComm.goBackward( 15 );
	      roombaComm.spinLeft(15);
  	      roombaComm.goForward(10);
        }
        else if (locstatus != 3 && locstatus != 4 && locstatus != 5) {
          System.out.println("Handling Maybe Left Maybe Right bump....");
  	      roombaComm.goBackward( 15 );
	      roombaComm.spinRight(15);
  	      roombaComm.goForward(10);
        }
        else if (locstatus == 3 || locstatus == 4 || locstatus == 5) {
          System.out.println("Handling Maybe Left Maybe Right bump....");
  	      roombaComm.goBackward( 15 );
	      roombaComm.spinLeft(15);
  	      roombaComm.goForward(10);
        }
/*       }
       catch(InterruptedException iex) {
      	 iex.printStackTrace();
       }*/
       roombaComm.updateSensors();
     }

     if(robotadjusted) {
       moveRemainDistance(roombaComm);
       synchronized (thread) { ((Robomain)thread).pleaseWait = false; thread.notify(); }
       System.out.println("Robothread given notification..........");
     }

     roombaComm.updateSensors();
    }

    System.out.println("Disconnecting");
    roombaComm.disconnect();

    System.out.println("Done");
  }

  synchronized void setTimeParams(boolean straight, int dist) {
    isStraight = straight;
    distance = dist;
    millisecs = Calendar.getInstance().getTimeInMillis();
  }

  synchronized static void setRemainDistance(RoombaCommSerial roombacomm) {
    int dist = 0;
    long msecs = Calendar.getInstance().getTimeInMillis();
    System.out.println("previousmili = " + millisecs);
    System.out.println("currentmili = " + msecs);
    millisecs = msecs - millisecs;
    dist = (int)((SPEED * millisecs)/1000);
    System.out.println("\n Distance = " + dist);
    millisecs = msecs;
    remdistance = distance - dist;
    remdistance = remdistance + 800;
  }

  synchronized static void moveRemainDistance(RoombaCommSerial roombacomm) {
    if(isStraight)
      	roombacomm.goStraight(remdistance);
    else
        roombacomm.goBackward(remdistance);

    remdistance = 0;
  }

  // Methods should be used strictly inside Robo thread.

  public void rtspinRight(int angle) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.spinRight(angle);
  }

  public void rtspinLeft(int angle) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.spinLeft(angle);
  }

  public void rtsetLocstatus(int ls) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    locstatus = ls;
  }

  public void rtsetTimeParams(boolean bool, int dt) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
     setTimeParams(bool, dt);
  }

  public void rtpause(int ms) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.pause(ms);
  }

  public void rtgoStraight(int dt) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.goStraight(dt);
  }

  public void rtgoBackward(int dt) {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.goBackward(dt);
  }

  public void rtstop() {
  	synchronized (this) {
  		while (pleaseWait) {
  			try { wait(); } catch (Exception e) { }
  	    }
  	}
    roombacomm.stop();
  }


  public void run() {

 /********************* Section to be modified *************/

//      roombacomm.goBackward( 200 ); // negative numbers not allowed
//      roombacomm.spinRight( 360 );  // negative numbers not allowed
//      roombacomm.spinLeft( 360 );     // negative numbers not allowed
//      roombacomm.pause(1000);

    rtsetLocstatus(-1);
    roombacomm.setSpeed(SPEED);     // can be positive or negative
    rtsetLocstatus(0);

    rtsetTimeParams(true, 1550);
    rtgoStraight( 1550);    // negative numbers not allowed
    rtspinRight( 90 );
    rtsetLocstatus(1);

    rtsetTimeParams(true, 1500);
	rtgoStraight( 1500);

	rtspinLeft(3);

	rtspinRight(1);

    rtpause(300);
    rtsetLocstatus(2);

    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft(3);

	rtspinRight(1);

    rtsetLocstatus(3);

    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft(3);

	rtspinRight(1);

    rtpause(300);
    rtsetLocstatus(4);

    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft(4);

	rtspinRight(1);

    rtsetLocstatus(5);
    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft(4);

	rtspinRight(1);

    rtpause(300);
    rtsetLocstatus(6);

    rtsetTimeParams(true, 2500);
	rtgoStraight( 2500);

	rtspinLeft( 90 );
    rtsetLocstatus(7);

    while(!roombacomm.bumpLeft() || !roombacomm.bumpRight()) {
       rtsetTimeParams(true, 200);
  	   rtgoStraight(200);
       //roombacomm.updateSensors();
    }

    rtsetTimeParams(false, 700);
	rtgoBackward( 700 );

	rtspinLeft (90);
    rtsetLocstatus(8);

    rtsetTimeParams(true, 1000);
	rtgoStraight(1000);

	rtspinRight(3);

	rtspinLeft(1);

	rtpause(200);

	rtsetTimeParams(true, 1000);
	rtgoStraight( 1000);

	rtspinRight(3);

	rtspinLeft(1);

    rtpause(300);

	rtspinRight(3);

	rtspinLeft(1);
    rtsetLocstatus(9);

    rtsetTimeParams(true, 2000);
	rtgoStraight( 2000);

	rtspinRight(5);

	rtspinLeft(1);
    rtsetLocstatus(10);

    rtsetTimeParams(true, 2000);
	rtgoStraight( 2000);

	rtspinLeft( 100 );

    rtpause(500);
    rtsetLocstatus(11);



    rtsetTimeParams(true, 2000);
	rtgoStraight(2000);

	rtspinRight(3);

	rtspinLeft(1);


    rtpause(200);
    rtsetLocstatus(12);

    rtsetTimeParams(true, 2000);
	rtgoStraight( 2000);

	rtspinRight(3);

	rtspinLeft(1);

    rtpause(200);
    rtsetLocstatus(13);

    rtsetTimeParams(true, 2000);
	rtgoStraight(2000);

	rtspinRight(3);

	rtspinLeft(1);

    rtsetLocstatus(14);

    rtsetTimeParams(true, 2000);
	rtgoStraight(2000);

    rtpause(200);

	rtspinRight(5);

	rtspinLeft(1);

    rtsetLocstatus(15);

    rtsetTimeParams(true, 1100);
	rtgoStraight(1100);

	rtpause(200);

	rtspinLeft(3);

	rtspinRight(1);

    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft( 100 );

    rtpause(200);
    rtsetLocstatus(16);

    rtsetTimeParams(true, 17000);
	rtgoStraight(1700);

	rtspinLeft(5);

	rtspinRight(1);

    rtsetLocstatus(17);
    rtsetTimeParams(true, 1500);
	rtgoStraight(1500);

	rtspinLeft(3);

	rtspinRight(1);
    rtsetLocstatus(18);

    rtsetTimeParams(true, 2200);
    rtgoStraight(2200);

	rtspinRight(2);

	rtspinLeft(1);

    rtsetLocstatus(19);
    rtsetTimeParams(true, 2300);
	rtgoStraight(2300);

    rtpause(100);

	rtspinLeft(53);

	rtspinRight(1);
    rtsetLocstatus(20);

    rtsetTimeParams(true, 2600);
	rtgoStraight(2600);

	rtspinRight(90);
    rtsetLocstatus(21);

    rtsetTimeParams(true, 600);
	rtgoStraight(600);

    rtsetLocstatus(22);

    rtstop();
    rtsetLocstatus(100);
    return;
  }

}
