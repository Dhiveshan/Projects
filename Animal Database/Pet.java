/** A class that represents one pet */
public class Pet {
	// FILL IN CODE: add instance variables (should be private!)
	// You need type, name, age, size, breed, gender, city, and
	// a boolean variable that has different meaning for
	// different types (for a cat, it means whether it's indoor or outdoor etc..)
	private String type;
	private String name;
	private String age;
	private String breed;
	private String size;
	private String gender;
	private boolean bool;
	private String city;
	
	private AnimalShelter shelter; // animal shelter where this pet is located

	/** Constructor of class Pet */
	public Pet(String type, String name, String age, String breed, String size, String gender, boolean bool, String city,
			AnimalShelter shelter) {
		// FILL IN CODE
		this.type = type;
		this.name = name;
		this.age = age;
		this.breed = breed;
		this.size = size;
		this.gender = gender;
		this.bool = bool;
		this.city = city;
		this.shelter = shelter;
	}

	// FILL IN CODE: add getters as needed
	public String getType()
	{
		return type;
	}
	public String getName()
	{
		return name;
	}
	public String getAge()
	{
		return age;
	}
	public String getBreed()
	{
		return breed;
	}
	public String getSize()
	{
		return size;
	}
	public String getGender()
	{
		return gender;
	}
	public boolean getBool()
	{
		return bool;
	}
	public String getCity()
	{
		return city;
	}
	public AnimalShelter getShelter()
	{
		return shelter;
	}
	/** Return a string in format described in the project description
	 * Example:
		Dog, Cracks, Poodle, young, small
		Shelter: Grateful Dogs Rescue,
		PO Box 411013,
		San Francisco 94141,
		(415) 587-112
	 */
	public String toString() {
		// FILL IN CODE

		return type + ", " + name + ", " + breed + ", " + age + ", " + size + System.lineSeparator() + shelter; // change it
	}

}