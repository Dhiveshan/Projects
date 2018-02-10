import java.util.ArrayList;
import java.util.Iterator;

/**
 * Class Profile. A superclass that stores the basic information contained in any
 * profile. Has two child classes: UserProfile and OrganizationProfile.
 **/
public class Profile {

	// FILL IN CODE: add instance variables

	/** Constructor of class Profile */
	public Profile(String name, String password, String image) {
		// FILL IN CODE
	}

	/** Create a Post with the given name and message and add it to the ArrayList of posts */
	public void addPost(String name, String message) {
		// FILL IN CODE
		
	}
	
	// FILL IN CODE: add getName() and a getter and a setter for the image file
	// Note: do not write getPosts methods that returns an ArrayList - it breaks encapsulation!

	public Iterator<Post> getPosts() {
		// return an iterator over the ArrayList of Posts 
		// FILL IN CODE
		return null; // change it
	}

	/** Return the string with the name associated with this profile */
	public String toString() {
		String s = "";
		// FILL IN CODE
		return s;
	}

	/** Return a String that contains all the posts  - this method will be user in UserProfile and OrganizationProfile*/
	public String getPostsAsString() {
		StringBuilder sb = new StringBuilder(); 
		
			Iterator<Post> it = getPosts();
			while (it.hasNext()) {
				// FILL IN CODE: append a message of each post 
				// and a newline character to the StringBuilder sb
			}
			return sb.toString();
		
	}

}
