package nfa;

/**
 * @author Unknown
 * Project 1 - NFA-Epsilon Simulator
 * Formal Languages
 *
 * This class represents an Event that is used to trigger a transition from State to State
 */
public class Event {
	private char theEventChar;

	/**
	 * Basic constructor
	 * @param event Character value used to trigger a transition
	 */
	public Event(char event)
	{
		theEventChar = event;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString()
	{
		return Character.toString(theEventChar);
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#equals(java.lang.Object)
	 */
	public boolean equals(Object anObj)
	{
		boolean isEqual = false;
		if(this == anObj)
		{
			isEqual = true;
		}
		else if((anObj instanceof Event) == true)
		{
			Event anEvt = (Event)anObj;
			if(anEvt.theEventChar == this.theEventChar)
			{
				isEqual = true;
			}
		}

		return isEqual;
	}
}
