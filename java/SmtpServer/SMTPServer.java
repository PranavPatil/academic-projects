import java.io.IOException;
import java.net.ServerSocket;

/**
 * Smtp3Server Creates an ServerSocket on a spezified port and waits for clients to be connected.
 * Then starts for every connection an Thread with an Smtp3Connection class.
 * The Server compares the host keyword from the mail.cfg configuration file
 * with the to statement from the client. Are they equal the E-Mail will be
 * local stored in the user directory. Are the not equal the E-Mail will be stored in the homeDir\Queue directory.
 * This can be run as an application !
 * @author (c) 1998/99 Computer Mutter computer@mutter.com ; Johannes Plachy
 * @version 0.99 11/99
 */
final public class SmtpServer extends Thread implements CFGUpdateable
{

   /**
    * default SMTP Port = 25
    */
   final private static int   DEFAULT_ServerSocket = 25;

   /**
    * open serversocket
    */
   private ServerSocket       ivSmtpSocket;

   /**
    * current SMTP Connection handler
    */
   private SmtpConnection     ivSmtpConnection;

   /**
    * current configuration
    */
   ConfigurationManager       ivConfigurationManager;

   /**
    * create new SMTP server with configuration data
    * @param configuration data
    */
   public SmtpServer( ConfigurationManager ConfigurationManager )
   {
      updateCFG( ConfigurationManager );

      try
      {
         setSmtpSocket( new ServerSocket( getConfiguration().getLocalSMTPServerSocket() ) );
      }
      catch ( IOException ioexception )
      {
         handleException( ioexception, "error cannot start" );

         return;     // System.exit(1);
      }

      start();
   }


   /**
    * update configuration data
    * @param new configuration data
    */
   final public void updateCFG( ConfigurationManager ConfigurationManager )
   {
      ivConfigurationManager = ConfigurationManager;
   }


   /**
    * retrieve configuration data
    * @returns configuration data
    */
   final private ConfigurationManager getConfiguration()
   {
      return ivConfigurationManager;
   }


   /**
    * returns default SMTP port
    * @returns 25
    */
   final static public int getDefaultSmtpPort()
   {
      return DEFAULT_ServerSocket;
   }


   /**
    * writes exception information to logfile
    * @param exception to be logged
    * @param more info about exception
    */
   final private void handleException( Throwable throwable, String Comment )
   {
      writeErrorMsg( throwable.toString() + " " + Comment );

      if ( Debug.enabled == true )
      {
         throwable.printStackTrace();
      }
   }


   /**
    * wait for conections and spins off SMTP connection handlers for each request
    */
   final public void run()
   {
      try
      {
         do
         {
            java.net.Socket   socket;

            try
            {
               if ( Debug.enabled )
               {
                  writeStatusMsg( "SMTP Server waiting for connection..." );
               }

               socket = getSmtpSocket().accept();
            }
            catch ( IOException ioexception )
            {
               handleException( ioexception, "error checking port: " + getConfiguration().getLocalSMTPServerSocket() );

               break;
            }

            if ( Debug.enabled )
            {
               writeStatusMsg( "SMTP Server starting new connectionhandler..." );
            }

            ivSmtpConnection = new SmtpConnection( ivConfigurationManager, socket );

            if ( ivSmtpConnection != null )
            {
               getConfiguration().getThreadPool().addTask( ivSmtpConnection );
            }
            else
            {
               writeErrorMsg( "ivSmtpConnection == null <- error" );
            }
         }
         while ( true );
      }
      catch ( Throwable exception )
      {
         handleException( exception, "Exception happened in SMTP run Loop" );
      }
   }


   /**
    * returns server socket for internal use
    * @returns open serversocket
    */
   ServerSocket getSmtpSocket()
   {
      return ivSmtpSocket;
   }


   /**
    * set new server socket for internal use
    * @param open serversocket
    */
   final private void setSmtpSocket( ServerSocket serversocket )
   {
      ivSmtpSocket = serversocket;
   }


   /**
    * write exception information to file
    * @param configuration
    */
//   final private void handleException( Throwable throwable )
//   {
//      writeErrorMsg( throwable.toString() );
//   }


   /**
    * write status message to logfile
    * @param line to be logged
    */
   final private void writeStatusMsg( String s )
   {
      ErrorReporter.getInstance().writeSmtpMsg( s );
   }


   /**
    * error message to be logged
    * @param line to be logged
    */
   final private void writeErrorMsg( String s )
   {
      ErrorReporter.getInstance().writeSmtpMsg( s, "Error" );
   }


   /**
    * utility function to instantiate a new server
    * @param coniguration data
    */
   final public static SmtpServer startServer( ConfigurationManager ConfigurationManager )
   {
      return new SmtpServer( ConfigurationManager );
   }


   /**
    * to run a standalone version of the SMTP server
    * @param hostname
    * @param homedirectoy
    */
   public static void main( String args[] )
   {
      System.out.println( "Java SmtpServer Version 0.8 (c) 1998 Computer Mutter" );
      System.out.println( "adapted 1999 by Johannes Plachy" );

      try
      {
         ConfigurationManager configurationManager = new ConfigurationManager( ConfigurationManager.getDefaultCFGFilename() );

         ErrorReporter.getInstance().updateCFG( configurationManager );
         startServer( configurationManager );
      }
      catch ( Exception exception )
      {
         System.err.println( exception );
      }
   }


}

