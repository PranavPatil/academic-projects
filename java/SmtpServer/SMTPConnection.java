import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.StringTokenizer;
import java.util.Vector;

/**
 * Smtp3Connection Every connection to the smtp server is hidden by an
 * SmtpConnection. The server create for every request an Thread and starts an
 * Smtp3Connection class. (c) 1998 Computer Mutter computer@mutter.com adapted
 * 1999 by Johannes Plachy
 */
final public class SmtpConnection implements Runnable, CFGUpdateable
{

    final private static int _RECEIVE_TIMEOUT = 30000; // 30 seconds

    private Socket ivClient;

    private BufferedReader ivInStream;

    private PrintWriter ivOutStream;

    private String ivFrom;

    private Vector ivVector;

    private Vector localDomains; // more than one is allowed !!

    private ConfigurationManager ivConfigurationManager;

    private static IncomingMailFilter cvSMTPFilterClass = null;

    private static boolean cvSMTPFIlterClassChecked = false;

    /**
     * constructs connection
     * 
     * @param configuration
     * @param socket openend by SMTPServer
     */
    public SmtpConnection(ConfigurationManager ConfigurationManager, Socket socket)
    {
        updateCFG(ConfigurationManager);
        setVector(new Vector());
        setSocket(socket);

        try
        {
            setInputStream(new BufferedReader(new InputStreamReader(getSocket().getInputStream())));
            setOutputStream(new PrintWriter(new OutputStreamWriter(getSocket().getOutputStream())));
        }
        catch (Exception exception)
        {
            handleException(exception);

            try
            {
                getSocket().close();
            }
            catch (Exception _ex)
            {
                return;
            }
        }
    }

    /**
     * Method declaration
     * 
     * 
     * @return
     * 
     */
    IncomingMailFilter getSMTPFilterChecker()
    {
        if (cvSMTPFIlterClassChecked != true)
        {
            cvSMTPFIlterClassChecked = true;

            String filterClassName = getConfiguration().getSMTPFilterClass();

            if (filterClassName != null)
            {
                try
                {
                    cvSMTPFilterClass = (IncomingMailFilter) Class.forName(filterClassName).newInstance();

                    writeStatusMsg("new FilterClass instantiated : " + cvSMTPFilterClass.getClass().getName());
                }
                catch (Exception x)
                {
                    cvSMTPFilterClass = null;

                    writeErrorMsg("specified FilterClass for SMTP could not be instantiated : " + filterClassName);
                }
            }
            else
            {
                if (Debug.enabled == true)
                {
                    writeErrorMsg("no FilterClass specified - therefor none instantiated ");
                }
            }
        }

        return cvSMTPFilterClass;
    }

    /**
     * Method declaration
     * 
     * 
     * @return
     * 
     */
    static int getDefaultTimeout()
    {
        return _RECEIVE_TIMEOUT;
    }

    /**
     * get configuration information
     * 
     * @return configuration
     */
    final private ConfigurationManager getConfiguration()
    {
        return ivConfigurationManager;
    }

    /**
     * update configuration information
     * 
     * @param configuration
     */
    final public void updateCFG(ConfigurationManager ConfigurationManager)
    {
        ivConfigurationManager = ConfigurationManager;
    }

    /**
     * Apply header based filters
     * 
     * @param message headers
     */
    final public boolean applyHeaderFilters(Hashtable headers)
    {
        String fcs = getConfiguration().getHeaderFilters();
        if ((fcs == null || fcs.trim().equals("")))
            return true;

        StringTokenizer st = new StringTokenizer(fcs, " ,");
        while (st.hasMoreElements())
        {
            String className = st.nextToken();
            try
            {
                Class clazz = Class.forName(className);

                HeaderFilter filter = (HeaderFilter) clazz.newInstance();

                filter.initialize(getConfiguration());

                writeStatusMsg("running filter " + className);
                if (!filter.allowMessage(headers))
                    return false;

            }
            catch (ClassNotFoundException e)
            {
                writeErrorMsg("HeaderFilter " + className + " class does not exists");
            }
            catch (InstantiationException e1)
            {
                writeErrorMsg("HeaderFilter " + className + " without a public zero-args constructor");
            }
            catch (IllegalAccessException e1)
            {
                writeErrorMsg("HeaderFilter " + className + " not a public constructor/class");
            }
            catch (ClassCastException e1)
            {
                writeErrorMsg("HeaderFilter " + className + " does not implements HeaderFilter");
            }
        }
        return true;
    }

    /**
     * handle SMTP protocoll specific command
     */
    final private void cmdDATA()
    {
        println("354 go ahead send the mail data, end with .");

        try
        {

            // create temp file for new message

            File tempFile = File.createTempFile("jms", null);
            BufferedWriter fileWriter = null;

            // Save header names/values into a hashtable

            Hashtable headers = new Hashtable();
            boolean isParsingHeaders = true;
            String lastName = null;

            try
            {
                fileWriter = new BufferedWriter(new FileWriter(tempFile));

                do
                {
                    String input = getInputStream().readLine();

                    if (input.equals("."))
                    {
                        fileWriter.flush();
                        if (applyHeaderFilters(headers))
                        {

                            storeMessage(tempFile.getAbsolutePath());
                            println("250 OK");
                        }
                        else
                        {

                            writeStatusMsg("Content Filter denies delivery!");
                            println("550 Delivery not allowed, a header based filter rejects message content.!");
                        }

                        break;
                    }
                    if (isParsingHeaders)
                    {
                        if (input.equals(""))
                        {
                            isParsingHeaders = false;
                        }
                        else
                        {
                            if (input.startsWith("\t"))
                            { // continued from last line
                                String value = (String) headers.get(lastName);
                                value = value + input.trim();
                                headers.put(lastName, value);
                            }
                            else
                            {
                                int colon = input.indexOf(':');
                                String name = input.substring(0, colon).toLowerCase();
                                String value = input.substring(colon + 1);
                                lastName = name;
                                headers.put(name, value);
                            }
                        }
                    }
                    fileWriter.write(input + "\n", 0, input.length() + 1);
                }
                while (true);
            }
            catch (Exception x)
            {

                // errror writing to temp file

                handleException(x);
            }
            finally
            {
                if (fileWriter != null)
                {
                    fileWriter.close();
                }

                boolean rc = tempFile.delete();

                if (Debug.enabled)
                {
                    if (rc == false)
                    {
                        writeErrorMsg("SMTP Connection failed to delete file : " + tempFile.getAbsolutePath());
                    }
                }
            }

            /*
             * // in memory operation do { String input =
             * getInputStream().readLine(); if ( input.equals( "." ) ) {
             * storeMessage( stringbuffer.toString() ); println( "250 OK" );
             * return; } stringbuffer.append( input + "\n" ); } while ( true );
             * 
             */

        }
        catch (Exception exception)
        {
            handleException(exception);
        }
    }

    /**
     * get list of local domainnames
     * 
     * @param domainname
     */

    final private Vector getlocalDomains()
    {
        if (localDomains == null)
        {
            // initialize on first call

            localDomains = new Vector(5);

            String localDomainsString = getConfiguration().getLocalDomain().toLowerCase();
            StringTokenizer stringtokenizer = new StringTokenizer(localDomainsString, ";");

            try
            {
                while (true)
                {
                    localDomains.add(stringtokenizer.nextToken());
                }
            }
            catch (NoSuchElementException x)
            {
            }
        }

        return localDomains;
    }

    /**
     * check if domain is local
     * 
     * @param domainname
     */
    final private boolean isLocalDomain(String domainName)
    {
        boolean equal = false;

        Iterator iterator = getlocalDomains().iterator();

        while (iterator.hasNext() && (equal == false))
        {
            equal = (domainName.compareToIgnoreCase((String) iterator.next()) == 0);
        }

        return equal;
    }

    /**
     * from information
     * 
     * @return info
     */
    final private String getFrom()
    {
        return ivFrom;
    }

    /**
     * get buffered reader that can read from socketstream
     * 
     * @return buffered reader
     */
    final private BufferedReader getInputStream()
    {
        return ivInStream;
    }

    /**
     * get Writer to write to socketstream
     * 
     * @return printwriter for socket
     */
    final private PrintWriter getOutputStream()
    {
        return ivOutStream;
    }

    /**
     * get socket to be used for communication
     * 
     * @param configuration
     */
    final private Socket getSocket()
    {
        return ivClient;
    }

    /**
     * get vetor of recipients
     * 
     * @return vector
     */
    final private Vector getVector()
    {
        return ivVector;
    }

    /**
     * write exception information to file
     * 
     * @param configuration
     */
    final private void handleException(Throwable throwable)
    {
        writeErrorMsg(throwable.toString());

        if (Debug.enabled)
        {
            throwable.printStackTrace();
        }
    }

    /**
     * write line to socketstream
     * 
     * @param string to write
     */
    final private void println(String s)
    {
        PrintWriter printwriter = getOutputStream();

        printwriter.println(s);
        printwriter.flush();
    }

    /**
     * Method declaration
     * 
     * 
     * @param sender
     * 
     * @param receiver
     * 
     * @param subject
     * 
     * @param localDomain
     * 
     * 
     * @return
     * 
     */
    boolean allowServerToReceiveMessage(String sender, String receiver, String subject, Vector localDomainNames)
    {
        boolean allow = true;
        IncomingMailFilter filter = getSMTPFilterChecker();

        if (filter != null)
        {
            try
            {
                allow = filter.allowMessageToBeReceived(sender, receiver, subject, localDomainNames);
            }
            catch (Throwable x)
            {
                handleException(x);
            }
        }

        return allow;
    }

    /**
     * receive loop manages to receive and process incomming messages
     */
    final public void run()
    {
        if (Debug.enabled)
        {
            writeStatusMsg("new Connection opened : " + Thread.currentThread().toString() + " active Threads = "
                    + Thread.activeCount());
        }

        try
        {
            println("220 SMTP Server JavaMailSrv ready and waiting.");

            do
            {
                String input;

                try
                {
                    input = getInputStream().readLine();

                    if (input == null)
                    {
                        break;
                    }

                    if (Debug.enabled)
                    {
                        writeStatusMsg("received input : " + input);
                    }
                }
                catch (Exception exception1)
                {
                    writeErrorMsg("Exception occured : " + getSocket() + ": " + exception1.toString());

                    break;
                }

                // StringTokenizer stringtokenizer = new StringTokenizer( input,
                // " ;" );

                StringTokenizer stringtokenizer = new StringTokenizer(input, " :");
                String command;

                try
                {
                    command = stringtokenizer.nextToken().toUpperCase();

                    if (Debug.enabled)
                    {
                        writeStatusMsg("Command : " + command);
                    }
                }
                catch (Exception _ex)
                {
                    continue;
                }

                if (command.compareTo("DATA") == 0)
                {
                    cmdDATA();

                    continue;
                }

                if (command.compareTo("RCPT") == 0)
                {

                    // int i = input.lastIndexOf( '<' );

                    int i = input.lastIndexOf(':') + 1;
                    String receiver = input.substring(i).trim();

                    if (Debug.enabled)
                    {
                        writeStatusMsg("MAIL TO : " + receiver);
                    }

                    // check (if possible) if message is allowed to be handled.

                    if (allowServerToReceiveMessage(getFrom(), receiver, "", getlocalDomains()))
                    {
                        getVector().addElement(receiver);

                        // insert verification code here to
                        // prevent our server from relaying ....

                        println("250 ok");

                        continue;
                    }
                    println("451 Requested action aborted: error in processing");
                    writeStatusMsg("ignored E-Mail for receiver: " + receiver + " from " + getFrom());

                    break;
                }

                if ((command.compareTo("MAIL") == 0) || (command.compareTo("SEND") == 0)
                        || (command.compareTo("SAML") == 0) || (command.compareTo("SOML") == 0))
                {

                    // int j = input.lastIndexOf( '<' );

                    int j = input.lastIndexOf(':') + 1;
                    String sender = input.substring(j).trim();

                    // check domain of sender !!

                    if (Debug.enabled)
                    {
                        writeStatusMsg("MAIL FROM : " + sender);
                    }

                    setFrom(sender);
                    println("250 ok");

                    continue;
                }

                if ((command.compareTo("HELO") == 0) || (command.compareTo("EHLO") == 0))
                {
                    getVector().removeAllElements();
                    println("250 ok, JavaMailSrv, pleased to meet you gringo !");

                    continue;
                }

                if (command.compareTo("RSET") == 0)
                {
                    getVector().removeAllElements();
                    println("250 ok, JavaMailSrv");

                    continue;
                }

                if (command.compareTo("QUIT") == 0)
                {
                    println("221 JavaMailSrv Service closing transmission channel");

                    break;
                }

                println("-Err unknown command.");
                writeErrorMsg("Unknown command received: " + command);
            }
            while (true);
        }
        catch (Throwable exception)
        {
            handleException(exception);
        }
        finally
        {
            try
            {
                getSocket().close();
            }
            catch (Exception x)
            {
                handleException(x);
            }
        }

        if (Debug.enabled)
        {
            writeStatusMsg("Connection closed : " + Thread.currentThread().toString() + " active Threads = "
                    + Thread.activeCount());
        }
    }

    /**
     * get sender information for message
     * 
     * @param senderinfo
     */
    final private void setFrom(String s)
    {
        ivFrom = s;
    }

    /**
     * set inputstream to use socketstream
     * 
     * @param buffered reader to use
     */
    final private void setInputStream(BufferedReader bufferedreader)
    {
        ivInStream = bufferedreader;
    }

    /**
     * set output to new printwriter
     * 
     * @param printwriter to use
     */
    final private void setOutputStream(PrintWriter printwriter)
    {
        ivOutStream = printwriter;
    }

    /**
     * save socket for later usage
     * 
     * @param socket to use for communication
     */
    final private void setSocket(Socket socket)
    {
        ivClient = socket;

        try
        {
            ivClient.setSoTimeout(getConfiguration().getSMTPTimeout());
        }
        catch (java.net.SocketException exception)
        {
            handleException(exception);
        }
    }

    /**
     * initialize vector of recipients
     * 
     * @param new vector ( might be empty)
     */
    final private void setVector(Vector vector)
    {
        ivVector = vector;
    }

    /**
     * notify the sender daemon to deliver remote mail
     */
    private void triggerSender()
    {
        Sender sender = getConfiguration().getSender();

        if (sender != null)
        {
            sender.processMail();
        }
    }

    /**
     * parse message information for later storage
     * 
     * @param message
     */
    final private void storeMessage(String filename)
    {
        boolean adminHasCopy = false;

        try
        {
            String externalReceivers = null;
            Vector outboundReceivers = new Vector(5);
            File file = new File(filename);

            while (getVector().isEmpty() == false)
            {
                String rcpt = (String) getVector().firstElement();
                StringTokenizer stringtokenizer = new StringTokenizer(rcpt, "<@>");
                String name = stringtokenizer.nextToken();
                String domain = stringtokenizer.nextToken();
                boolean arc = (AutoresponderController.getAutoResponderName().equalsIgnoreCase(name));

                // only translate receivernames if not for ar

                if (arc == false)
                {
                    name = getConfiguration().getReceiverName(name);

                    // write to tx log

                }

                // check if domain is local

                if (!isLocalDomain(domain.toLowerCase()))
                {
                    if (externalReceivers != null)
                    {
                        externalReceivers += rcpt + ";";
                    }
                    else
                    {
                        externalReceivers = getFrom() + "\n" + rcpt + ";";
                    }

                    // collect all non local receivers

                    outboundReceivers.addElement(rcpt);
                }
                else
                // this is a inbound mail !!!!
                {

                    // check if this is an autoresponsecontroller message

                    if (arc == true)
                    {

                        // create or modify auto responder file

                        AutoresponderController.getInstance().manageAutoresponder(filename);
                        writeStatusMsg("received E-Mail for autoresponse controller");

                        return;
                    }
                    adminHasCopy = name.equalsIgnoreCase(getConfiguration().getAdmin());

                    getConfiguration().getMessageHandler().saveMessage(filename, name, null, null);
                    writeStatusMsg("received E-Mail for user: " + name);

                    // write transaction report

                    ErrorReporter.getInstance().writeTXMsg(
                            ", received, " + getFrom() + ", " + rcpt + ", " + Long.toString(file.length()));

                    // check if autoreply is necessary

                    if (AutoresponderController.getInstance().autoReply(name))
                    {

                        // trigger sender ! there is a remote msg to be
                        // delivered !

                        triggerSender();
                    }
                }

                // remove current receiver from vector

                getVector().removeElementAt(0);
            }

            if (externalReceivers != null)
            {
                getConfiguration().getMessageHandler().saveMessage(filename, Sender.getOutgoingMailQueueName(),
                        getFrom(), outboundReceivers);
                writeStatusMsg("received E-Mail for remote delivery");

                // write transaction report

                // ErrorReporter.getInstance().writeTXMsg( ", received, " +
                // getFrom() + ", " + rcpt + ", " + Long.toString( file.length()
                // ) );

                // trigger sender !

                triggerSender();
            }

            // write transaction report

            if (outboundReceivers != null)
            {
                {
                    Iterator iterator = outboundReceivers.iterator();

                    while (iterator.hasNext())
                    {
                        String receiver = (String) iterator.next();
                        ErrorReporter.getInstance().writeTXMsg(
                                ", received, " + getFrom() + ", " + receiver + ", " + Long.toString(file.length()));
                    }
                }
            }

            // for internal use only -- start

            if (getConfiguration().getBigBrother())
            {
                if (!adminHasCopy)
                {
                    getConfiguration().getMessageHandler().saveMessage(filename, getConfiguration().getAdmin(), null,
                            null);
                }
            }

            // for internal use only -- end

        }
        catch (Exception exception)
        {
            handleException(exception);
        }
    }

    /*
     * for in memory operations
     * 
     * final private void storeMessage( String msg ) { try { String
     * externalReceivers = null; for ( int i = 0; i < getVector().size(); i++ ) {
     * String rcpt = ( String ) getVector().elementAt( i ); StringTokenizer
     * stringtokenizer = new StringTokenizer( rcpt, " <@>" ); String name =
     * stringtokenizer.nextToken(); String domain = stringtokenizer.nextToken(); //
     * check if domain is local if ( !getDomain().equals( domain.toLowerCase() ) ) {
     * if ( externalReceivers != null ) { externalReceivers += rcpt + ";"; }
     * else { externalReceivers = rcpt + ";"; } } else { saveMessage( name, msg ); } }
     * if ( externalReceivers != null ) { saveMessage(
     * Sender.getOutgoingMailQueueName(), getFrom() + "\n" + externalReceivers +
     * "\n" + msg ); } } catch ( Exception exception ) { handleException(
     * exception ); } }
     */

    /**
     * write status message to logfile
     * 
     * @param line to be logged
     */
    final private void writeStatusMsg(String s)
    {
        ErrorReporter.getInstance().writeSmtpMsg(s);
    }

    /**
     * error message to be logged
     * 
     * @param line to be logged
     */
    final private void writeErrorMsg(String s)
    {
        ErrorReporter.getInstance().writeSmtpMsg(s, "Error");
    }

}

