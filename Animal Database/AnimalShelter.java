/** A class that represents one animal shelter. */
public class AnimalShelter {
	// FILL IN CODE: add instance variables: name, shelterAddress, 
	// phone, description
	private String name;
	private Address shelterAddress;
	private String phone;
	private String description;

	/** Constructor */
	public AnimalShelter(String name, Address addr, String phone, String descr) {
		// FILL IN CODE
		this.name = name;
		this.shelterAddress = addr;
		this.phone = phone;
		this.description =descr;
	}

	public String getName()
	{
		return name;
	}
	public Address getAddress()
	{
		return shelterAddress;
	}
	public String getPhone()
	{
		return phone;
	}
	public String getDescription()
	{
		return description;
	}
	/** Should return a string in the following format:
	 * name followed by the comma on one line
	 * address on the next line
	 * phone number on the third line 
	 */
	public String toString() {
		// FILL IN CODE
		String formatN = name + ", " + "\n";


		return formatN + shelterAddress + "\n" + phone; // change it
	}

}