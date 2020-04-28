package nfa;

/**
 * @author Unknown
 * Project 1 - NFA-Epsilon Simulator
 * Formal Languages
 *
 * This class represents a Transition within the StateMachine
 */
public class Transition {
	private State theNextState = null;
	private Event theEvent = null;

	/**
	 * Basic constructor
	 * @param aNextState The state after transition is trigger
	 * @param aEvent The event that triggers the transition
	 */
	public Transition(State aNextState, Event aEvent)
	{
		if(aNextState == null)
		{
			throw new NullPointerException("Null state specified");
		}

		if(aEvent == null)
		{
			throw new NullPointerException("Null event specified");
		}
		theNextState = aNextState;
		theEvent = aEvent;
	}

	/**
	 * Gets the next state
	 * @return The transition's next state
	 */
	public State getNextState()
	{
		return theNextState;
	}

	/**
	 * Gets the triggering event
	 * @return The transition's trigger event
	 */
	public Event getEvent()
	{
		return theEvent;
	}
}
