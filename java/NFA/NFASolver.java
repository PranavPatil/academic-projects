
package nfa;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;
import java.util.zip.DataFormatException;


/**
 *
 * @author Unknown
 */
public class NFASolver {


    private boolean Machineready = false;
    private final String CMD_INPUT = "i";
    private final String CMD_RUN = "r";
    private final String CMD_EXT = "x";


    private CreateNFA nfa = new CreateNFA();
    private StateMachine Machine = null;


    /**
     * @param args the command line arguments
     */
   
    /**
	 * Main simulation entry point / processing loop
	 * @param aParmSet String array containing command-line parameters
	 */
	public void runMachine(String[] argv)
	{
		File inputFile = null;
                inputFile = new File(argv[0]);
		createMachine(inputFile);
                String commands = null;
		if(Machine != null)
		{
                        System.out.println("Machine Active:");
			Machineready = true;
			try
			{
				BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
				
				while(Machineready)
				{
					commands = null;
					System.out.print("Enter command here:");
					commands = br.readLine();
					processCommandLine(commands);
				}
			}
			catch (Exception e)
			{
				System.out.println("Unknown error occurred.  Exiting");
				e.printStackTrace();
			}
		}
	}

	


	private void processCommandLine(String aCommand)
	{
		if(aCommand == null || aCommand.length() == 0)
		{ // No command entered - show menu
			commandMenu();
		}
		else
		{
			aCommand = aCommand.trim();
			String[] cmds = aCommand.split(" ");

			if(cmds[0].compareToIgnoreCase(CMD_EXT) == 0)
			{ // Exit command
				Machineready = false;
			}
			else if(cmds[0].compareToIgnoreCase(CMD_INPUT) == 0 && cmds.length == 2)
			{ // Non-Empty file name/path (Cannot contain spaces)
				createMachine(new File(cmds[1].trim()));
			}
			else if(cmds[0].compareToIgnoreCase(CMD_RUN) == 0)
			{
				if(cmds.length == 2)
				{ // Non-Empty input string - r <input string>
					executeMachine(cmds[1].trim());
				}
				else if(cmds.length == 1)
				{ // Empty input string - r
					executeMachine(new String());
				}
				else
				{ // Invalid input [Contains spaces] - r <input string> <input string> etc
					System.out.println("Unknown command - Choose from the following :\n");
					commandMenu();
				}
			}
			else
			{ // Unknown command
				System.out.println("Unknown command - Choose from the following :\n");
				commandMenu();
			}
		}
	}

	
	private void createMachine(File aConfigFile)
	{
		try
		{
			Machine = nfa.createFromFile(aConfigFile);
		}
		
		catch (Exception e)
		{
			System.out.println("Unable to create machine");
			e.printStackTrace();
		}
	}

	
	private void executeMachine(String aEventString)
	{
		
			try
			{
				if(Machine.processInputString(aEventString) == true)
				{
					System.out.println("Event string accepted by state machine.");
					System.out.println("State performed the following transitions: \n");

					State cState = Machine.getInitialState();
					Stack<Transition> transitions = Machine.getPerformedTransitions();
					Transition cTrans = null;
					while(transitions.isEmpty() == false)
					{
						cTrans = transitions.pop();
						System.out.println("(" + cState.getName() + "," +
								cTrans.getEvent().toString() + ")->" +
								cTrans.getNextState().getName());
						cState = cTrans.getNextState();
					}
				}
				else
				{
					System.out.println("Event string rejected by state machine.");
				}
			}
			catch (NullPointerException e)
			{
				System.out.println("Unable to process event string due to following error: ");
				System.out.println(e.getMessage());
			}
		}
	


    
	private void commandMenu()
	{
                
		System.out.println("Please enter the filepath of the file that describes the machine rules.");
		System.out.println("\t'i <Name + File Path>'");
		System.out.println("To pass an event string to the state machine:");
		System.out.println("\t'r <Event String>'");
		System.out.println("To Exit:");
		System.out.println("\t'x'");
	}

         public static void main(String[] argv)
    {
        System.out.println("Welcome to the NFA Solver Application");
        NFASolver nfa = new NFASolver();
	nfa.runMachine(argv);

    }



}
