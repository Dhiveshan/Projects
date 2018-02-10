/** A class that represents a profile of an organization */
public class OrganizationProfile extends Profile {
	// FILL IN CODE: declare instance variables

	/** Constructor of the class */
	public OrganizationProfile(String name, String password, String image, String phone, String addr, int likes) {
		super(name, password, image);
		// FILL IN CODE

	}

	// FILL IN CODE: write getters for getPhone, getAddress()
	public String getPhone() {
		String s = "";
		// FILL IN CODE

		return s; // change
	}

	public String getAddress() {
		String s = "";
		// FILL IN CODE
		return s;
	}

	/**
	 * Add this person as a supporter for the organization. Do not add as a
	 * supporter if person is actually an organization. Do not add as a
	 * supporter if the person does not have a profile on the social network.
	 */
	public boolean addSupporter(String person, SocialNetwork n) {
		// FILL IN CODE
		return false; // change

	}

	/** Remove a given supporter from the list of supporters */
	public boolean removeSupporter(String friend) {
		// FILL IN CODE
		return false; // change
	}

	/** Return a String representing this organization's profile */
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(super.toString()); // just the name
		// FILL IN CODE: 
			// append address and phone
			// append events (posts)
			// append supporters
		
	
		return sb.toString();
	}
}
