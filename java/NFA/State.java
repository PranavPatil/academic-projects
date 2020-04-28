package nfa;

import java.util.LinkedList;
import java.util.List;
import java.util.zip.DataFormatException;

/**
 * @author Unknown
 * Project 1 - NFA-Epsilon Simulator
 * Formal Languages
 *
 * This class represents a State within the StateMachine
 */
public class State {
	private final static String DEFAULT_NAME = "DEFAULT";

	private LinkedList<Transition> theTransitions = new LinkedList<Transition>(); ;
	private boolean isFinal = false;
	private String theName;

	/**
	 * Default Constructor
	 * @throws NullPointerException
	 * @throws DataFormatException
	 */
	public State() throws NullPointerException, DataFormatException
	{
		this(DEFAULT_NAME);
	}

	/**
	 * Constructor that sets Name value
	 * @param aName The name of the state
	 * @throws NullPointerException If aName is null
	 * @throws DataFormatException If aName is improperly formatted
	 */
	public State(String aName) throws NullPointerException, DataFormatException
	{
		this(aName, false);
	}

	/**
	 * Constructor that sets Name and Final values
	 * @param aName The name of the state
	 * @param isFinal True if the state is a final state
	 * @throws NullPointerException If aName is null
	 * @throws DataFormatException If aName is improperly formatted
	 */
	public State(String aName, boolean isFinal) throws NullPointerException, DataFormatException
	{
		if(aName == null)
		{
			throw new NullPointerException("Null State Name specified");
		}

		if(aName.length() == 0)
		{
			throw new DataFormatException("Invalid State Name specified");
		}

		theName = aName;
		this.isFinal = isFinal;
	}

	/**
	 * Adds transition to state
	 * @param aTransition The transition to add
	 * @throws NullPointerException If aTransition is null
	 */
	public void addTransition(Transition aTransition) throws NullPointerException
	{
		if(aTransition == null)
		{
			throw new NullPointerException("null Transition specified");
		}

		theTransitions.add(aTransition);

	}

	/**
	 * Retrieves the transitions attached to this state
	 * @return The attached transitions
	 */
	public List<Transition> getTransitions()
	{
		return theTransitions;
	}

	/**
	 * Gets the name of the state
	 * @return The state name
	 */
	public String getName()
	{
		return theName;
	}

	/**
	 * Gets the final status of the state
	 * @return The final status
	 */
	public boolean isFinal()
	{
		return isFinal;
	}

	/**
	 * Sets the final status of the state
	 * @param isFinal The final status
	 */
	public void makeFinal(boolean isFinal)
	{
		this.isFinal = isFinal;
	}
}

