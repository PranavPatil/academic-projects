package nfa;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Stack;

/**
 * @author Unknown
 * Project 1 - NFA-Epsilon Simulator
 * Formal Languages
 *
 * This class represents the StateMachine
 */
public class StateMachine {
	private State theInitialState = null;
	private Stack<Event> theUnprocessedEvents;
	private Stack<Event> theProcessedEvents;
	private Stack<Transition> thePerformedTransitions;
	private Event theEpsilonEvent;
	private HashMap<State, Stack<Integer>> theVisitedMap;


	/**
	 * Basic Constructor
	 * @param aInitialState The starting state to be used by this machine
	 * @param aEpsilonEvent The event to be used for EPSILON-transitions for this machine
	 */
	public StateMachine(State aInitialState, Event aEpsilonEvent)
	{
		theInitialState = aInitialState;
		theEpsilonEvent = aEpsilonEvent;
	}

	/**
	 * Sets the initial state for this machine
	 * @param aState The initial state
	 */
	public void setInitialState(State aState)
	{
		theInitialState = aState;
	}

	/**
	 * Sets the event to be used for EPSILON-transitions for this machine
	 * @param aEpsilonEvent The epsilon-event
	 */
	public void setEpsilonEvent(Event aEpsilonEvent)
	{
		theEpsilonEvent = aEpsilonEvent;
	}

	/**
	 * Gets the initial state for this machine
	 * @return The initial state
	 */
	public State getInitialState()
	{
		return theInitialState;
	}

	/**
	 * Gets the event used for EPSILON-transitions
	 * @return The EPSILON-event
	 */
	public Event getEpsilonEvent()
	{
		return theEpsilonEvent;
	}

	/**
	 * Gets the Transitions performed during the last execution of this machine
	 * @return List containing transitions
	 */
	public Stack<Transition> getPerformedTransitions()
	{
		return (Stack<Transition>) thePerformedTransitions.clone();
	}

	/**
	 * Executes the machine on the given input string
	 * @param input The string to be processed
	 * @return true If the string was accepted
	 * @throws NullPointerException if input is null or the Initial State is null
	 */
	public boolean processInputString(String input) throws NullPointerException
	{
		if(theInitialState == null)
		{
			throw new NullPointerException("null Initial State");
		}

		if(input == null)
		{
			throw new NullPointerException("null Input string");
		}

		theUnprocessedEvents = new Stack<Event>();
		theProcessedEvents = new Stack<Event>();
		thePerformedTransitions = new Stack<Transition>();
		theVisitedMap = new HashMap<State, Stack<Integer>>();

		createEvents(input);
		return processEvents(theInitialState);
	}

	/**
	 * Converts the a String into Events
	 * @param input The to-be-converted string
	 */
	private void createEvents(String input)
	{
		for(int i = input.length() - 1; i >= 0; --i)
		{
			theUnprocessedEvents.push(new Event(input.charAt(i)));
		}
	}

	/**
	 * The recursive event-processor
	 * @param aCurrentState
	 * @return true if we are in a final state and there are no more events to process
	 */
	private boolean processEvents(State aCurrentState)
	{
		boolean processingComplete = false;

		if(theUnprocessedEvents.size() == 0)
		{ // No events left to process, determine if we are in a final state
			processingComplete = aCurrentState.isFinal();
		}

		if(processingComplete == false)
		{ 	// There are still events to process or we are not in a final state and there may be
			// EPSILON transitions that will get us there

			boolean processEvents = true;
			Stack<Integer> remainingEvents = theVisitedMap.get(aCurrentState);

			if(remainingEvents != null)
			{ // An attempt to visit this State has occurred before
				if(remainingEvents.isEmpty() == false
						&& remainingEvents.peek().intValue() == theUnprocessedEvents.size())
				{	// State has been visited and an EPSILON loop exists
					processEvents = false;
				}
			}
			else

			{ // first attempt to visit State
				theVisitedMap.put(aCurrentState, new Stack<Integer>());
			}

			if(processEvents)
			{ // Events remain to be processed / or an EPSILON loop does not exist
				Iterator<Transition> iter = aCurrentState.getTransitions().listIterator();

				while(iter.hasNext() && processingComplete == false)
				{ // Perform transitions until we have run out of them or have found a final state

					Transition currentTransition = iter.next();
					if(currentTransition.getEvent().equals(theEpsilonEvent))
					{  // EPSILON transition - no event is consumed
						theVisitedMap.get(aCurrentState).push(new Integer(theUnprocessedEvents.size()));
						theProcessedEvents.push(theEpsilonEvent);
						processingComplete = processEvents(currentTransition.getNextState());
					}
					else if(theUnprocessedEvents.isEmpty() == false)
					{ // Can only perform non-EPSILON transitions if events remain to be processed
						if(currentTransition.getEvent().equals(theUnprocessedEvents.peek()))
						{  // Non-EPSILON transition - event is consumed
							theVisitedMap.get(aCurrentState).push(new Integer(theUnprocessedEvents.size()));
							theProcessedEvents.push(theUnprocessedEvents.pop());
							processingComplete = processEvents(currentTransition.getNextState());

							if(processingComplete == false)
							{ // Did not find final state, therefore event was not really consumed
								theUnprocessedEvents.push(theProcessedEvents.pop());
							}
						}
					}

					if(processingComplete)
					{ // Found a final state, record transition for output
						thePerformedTransitions.push(currentTransition);
					}
				}
			}
		}

		return processingComplete;
	}
}
