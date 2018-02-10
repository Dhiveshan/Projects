
/*
 * File: UserProfile.java
 * ------------------------------
 * Profile of a person on the social network. Extends class Profile.
 * Contains a list of friends, and a list of organizations this user supports.
 */

import java.util.*;

public class UserProfile extends Profile {

	// FILL IN CODE: add instance variables: friends and organizations (both
	// ArrayLists of Strings)

	public UserProfile(String name, String password, String image) {
		super(name, password, image);
	}

	/**
	 * This method adds the named friend to this profile's list of friends. It
	 * returns true if the friend's name was not already in the list of friends
	 * for this profile (and the name is added to the list). The method returns
	 * false if the given friend name was already in the list of friends for
	 * this profile (in which case, the given friend name is not added to the
	 * list of friends a second time.)
	 */
	public boolean addFriend(String friend, SocialNetwork n) {
		// FILL IN CODE
		return false; // change

	}

	/**
	 * This method removes the named friend from this profile's list of friends.
	 * It returns true if the friend's name was in the list of friends for this
	 * profile (and the name was removed from the list). The method returns
	 * false if the given friend name was not in the list of friends for this
	 * profile (in which case, the given friend name could not be removed.)
	 */
	public boolean removeFriend(String friend) {
		// FILL IN CODE
		return false; // change
	}

	public boolean addOrganization(String name, SocialNetwork n) {
		Profile p = n.getProfile(name);
		if (p == null) {
			System.out.println("No profile for this organization, can't add it for this user.");
			return false;
		}

		// FILL IN CODE: Add a name to the list of organizations that this user
		// supports (likes)
		return false; // change
	}

	/**
	 * This method returns a string representation of the user profile. This
	   string is of the form (this is just an example): 
	   Xiaofeng 
	   Posts: 
	   Going to play soccer with a some friends 
	   Anybody knows a decent sushi place?
	   Friends: 
	   Cody, Pablo 
	   Supports the following organizations:
	   San Francisco Opera
	   
	   There may be more than 2 posts. For the organizations, multiple ones would be separated by comma
	 */
	public String toString() {
		// Use a StringBuilder, append all info to the StringBuilder, then call
		// sb.toString() to
		// return a string
		StringBuilder sb = new StringBuilder();
		// FILL IN CODE

		return sb.toString();
	}

	// FILL IN: Feel free to add some helper methods as needed

}
