/*
 * File: SocialNetwork.java
 * -------------------------------
 * This class keeps track of the profiles of all people and organizations.
 */

import java.util.*;

public class SocialNetwork {

	private Map<String, Profile> users;

	public SocialNetwork() {
		users = new HashMap<String, Profile>();
	}

	/** Read profiles from the file and add info to the HashMap.
	 *  You are required to catch exceptions here.
	 *  When you create a profile, create a password by concatenating 3 low case
	 *  letters of the name and the year. */
	public void readProfiles(String filename) {
		// FILL IN CODE
		
	}

	

	/**
	 * This method returns the profile associated with the given name in the
	 * HashMap. If there is no profile in the HashMap with the given name, the
	 * method returns null.
	 */
	public Profile getProfile(String name) {
		// FILL IN CODE
		return null; // Change
	}

}
