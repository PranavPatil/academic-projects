/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package nfa;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.zip.DataFormatException;

/**
 *
 *@author Unknown
 */
public class CreateNFA {

private String EPSILON_TOKEN = "EPSILON-EVENT";
	private String IS_TOKEN = "INITIAL-STATE";
	private String FS_TOKEN = "FINAL-STATE";
	private String TRANS_TOKEN = "TRANSITION";
	private String DEFIN_DELIM = ":";
	private String TRANS_DELIM = ",";
	private int DEFIN_TOKENS = 2;
	private int TRANS_TOKENS = 3;
	private char COMMENT_DELIM = '#';
	private HashMap<String, State> theStateMap = null;

        /**
	 * Default Constructor
	 */
	public CreateNFA()
	{

	}

        /**
	 * Creates a StateMachine from the model contained within aFile
	 * @param aFile The file containing the state machine model
	 * @return The StateMachine created from the state machine model
	 * @throws DataFormatException If aFile is improperly formatted
	 * @throws NullPointerException If aFile is null
	 * @throws IOException If aFile cannot be read
	 */
	public StateMachine createFromFile(File aFile) throws DataFormatException, NullPointerException, IOException
	{
		StateMachine createdMachine = null;
		Event epsilon = null;
		State initialState = null;

		theStateMap = new HashMap<String, State>();
		FileReader fr = new FileReader(aFile);
		BufferedReader br = new BufferedReader(fr);
		boolean finalSpecified = false;

		while(br.ready())
		{
			String inputLine = br.readLine().trim();

			if(inputLine.length() == 0)
			{ // Is empty? Ignore

			}
			else if(inputLine.charAt(0) == COMMENT_DELIM)
			{ // Is comment. Ignore

			}
			else
			{ // Not comment. Determine what type of input line (and if valid)
				String[] tokens = inputLine.split(DEFIN_DELIM);

				if(tokens.length == DEFIN_TOKENS)
				{
					String defineToken = tokens[0].trim();
					String valueToken = tokens[1].trim();

					if(defineToken.compareToIgnoreCase(IS_TOKEN) == 0 && initialState == null)
					{ // Initial-State: Token found
						initialState = getState(valueToken);
					}
					else if(defineToken.compareToIgnoreCase(FS_TOKEN) == 0)
					{ // Final-State: Token found
						State finalState = getState(valueToken);
						finalState.makeFinal(true);
						finalSpecified = true;
					}
					else if(defineToken.compareToIgnoreCase(TRANS_TOKEN) == 0)
					{ // Transition: Token found
						tokens = valueToken.split(TRANS_DELIM);

						if(tokens.length == TRANS_TOKENS)
						{
							createTransition(tokens[0].trim(), tokens[1].trim(), tokens[2].trim());
						}
						else
						{
							throw new DataFormatException("Unrecognized Transition Definition - " + inputLine);
						}
					}
					else if(defineToken.compareToIgnoreCase(EPSILON_TOKEN) == 0 && epsilon == null)
					{ // Epsilon-Value: Token found
						epsilon = createEvent(valueToken.trim());
					}
					else
					{
						throw new DataFormatException("Unrecognized Definition - " + inputLine);
					}
				}
				else
				{
					throw new DataFormatException("Unrecognized Input - " + inputLine);
				}
			}
		}

		if(initialState == null)
		{
				throw new DataFormatException("Initial State not specified - unable to create state machine.");
		}
		else
		{
			if(finalSpecified == false)
			{
				System.out.println("Warning specified state machine does not contain any final states.");
			}

			if(epsilon == null)
			{
				System.out.println("Warning specified state machine does not contain epsilon events - will behave as DFA.");
			}

			createdMachine = new StateMachine(initialState, epsilon);
		}

		return createdMachine;
	}

	/**
	 * Finds the State within the model with the specified name
	 * If a State does not exist with the given name, it is created
	 * @param aName The name of the state
	 * @return The existing / newly created State
	 * @throws DataFormatException If an empty (or invalid) name is specified
	 * @throws NullPointerException If aName is null
	 */
	private State getState(String aName) throws DataFormatException, NullPointerException
	{
		if(aName == null)
		{
			throw new NullPointerException("Null State Name specified");
		}

		State retrievedState = null;

		aName = aName.trim();
		if(aName.length() == 0)
		{
			throw new DataFormatException("Invalid State Name specified");
		}
		else
		{
			retrievedState = theStateMap.get(aName);

			if(retrievedState == null)
			{ // First time state has been found in file
				retrievedState = new State(aName);
				theStateMap.put(aName, retrievedState);
			}
		}

		return retrievedState;
	}

	/**
	 * Creates a transition for the given state / event triple.
	 * @param startName Name of the starting state
	 * @param eventChar Event that triggers the transition
	 * @param endName Name of the ending state
	 * @return The newly created Transition
	 * @throws DataFormatException If a state name or event is improperly formatted
	 * @throws NullPointerException If a state name or event is null
	 */
	private Transition createTransition(String startName, String eventChar, String endName) throws DataFormatException, NullPointerException
	{
		Transition createdTrans = null;
		Event event = createEvent(eventChar);

		State start = getState(startName);
		State end = getState(endName);
		createdTrans = new Transition(end, event);
		start.addTransition(createdTrans);

		return createdTrans;
	}

	/**
	 * Creates an event from the specified String
	 * @param eventChar String to base the event upon
	 * @return The newly created Event
	 * @throws DataFormatException If eventChar is improperly formatted
	 * @throws NullPointerException If eventChar is null
	 */
	private Event createEvent(String eventChar) throws DataFormatException, NullPointerException
	{
		if(eventChar == null)
		{
			throw new NullPointerException("Null Event Character specified");
		}

		Event createdEvent = null;
		eventChar = eventChar.trim();

		if(eventChar.length() != 1)
		{
			throw new DataFormatException("Invalid Event specified");
		}
		else
		{
			createdEvent = new Event(eventChar.charAt(0));
		}

		return createdEvent;
	}
/**
	 * Creates a StateMachine from the model contained within aFile
	 * @param aFile The file containing the state machine model
	 * @return The StateMachine created from the state machine model
	 * @throws DataFormatException If aFile is improperly formatted
	 * @throws NullPointerException If aFile is null
	 * @throws IOException If aFile cannot be read
	 */
}