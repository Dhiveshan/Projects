
/** A class that represents a "Pet Database" - stores a list of pets */
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.io.IOException;
public class PetDatabase {
	private ArrayList<Pet> pets; // an ArrayList of pets
	// FILL IN CODE: add lists for breeds
	private ArrayList<String> dogBreeds;
	private ArrayList<String> catBreeds;
	private ArrayList<String> birdBreeds;
	// You might also want to add a helper method to return
	// a list of breeds for a particular pet
	public ArrayList<String> getDogBreeds()
	{
		return dogBreeds;
	}
	public ArrayList<String> getCatBreeds()
	{
		return catBreeds;
	}
	public ArrayList<String> getBirdBreeds()
	{
		return birdBreeds;
	}


	/** Constructor. Calls readPets method. */
	public PetDatabase(String filename, ShelterDatabase db) {
		pets = new ArrayList<Pet>();
		dogBreeds = new ArrayList<String>();
		catBreeds = new ArrayList<String>();
		birdBreeds = new ArrayList<String>();
		readPets(filename, db);
	
	}

	/** Add a given pet to the list of pets */
	public void addPet(Pet a) {
		// FILL IN CODE
		pets.add(a);
	}

	public static void displayPets(ArrayList<Pet> foundPets) {
		System.out.println("Found the following pets: ");
		System.out.println("------------------------");
		// FILL IN CODE to print the pets we found
		for(int i =0;i<foundPets.size();i++)
		{
			System.out.println(foundPets.get(i));

		}
		
	}

	/**
	 * Return a list of pets that match given parameters.
	 * 
	 * @param type
	 * @param breed
	 * @param age
	 * @param size
	 * @param gender
	 * @param yesNoValue
	 * @param city
	 * @return
	 */
	public ArrayList<Pet> findPets(String type, String breed, String age, String size, String gender, String yesNoValue,
			String city) {
		// FILL IN CODE
		ArrayList<Pet> newPets = new ArrayList<Pet>();
		for(int i=0; i<pets.size(); i++)
		{
			if(type.contains(pets.get(i).getType()) || type.equals(""))
			{
				if(breed.contains(pets.get(i).getBreed()) || breed.equals(""))
				{
					if(age.contains(pets.get(i).getAge()) || age.equals(""))
					{
						if(size.contains(pets.get(i).getSize()) || size.equals(""))
						{
							if(gender.contains(pets.get(i).getGender()) || gender.equals(""))
							{
								if(yesNoValue.equals(pets.get(i).getBool()) || yesNoValue.equals(""))
								{
									if(city.contains(pets.get(i).getCity()) || city.equals(""))
									{
										newPets.add(pets.get(i));
									}
								}
							}
						}
					}
				}
			}
		}

		return newPets; // change it
	}

	/**
	 * Read pets from the file and add to the ArrayList of pets. A reference to
	 * the ShelterDatabase is needed so that we can find the shelter with a
	 * given name and store the reference to this shelter in the Pet object. You
	 * are required to handle IOException in this method using try/catch.
	 */
	public void readPets(String filename, ShelterDatabase shelters) {
		// FILL IN CODE
		try
		{
			FileReader petFile = new FileReader(filename);
			BufferedReader br = new BufferedReader(petFile);
			String line;
			
			while((line = br.readLine()) != null)
			{
				String[] s;
				String type;
				String breed;
				String name;
				String size;
				String age;
				String gender;
				String yesNoValue;
				String city;
				String shelter;
				String conditional;
				Boolean bool = true;
				s = line.split(",");
				type = s[0];
				breed = s[1];
				name = s[2];
				size = s[3];
				age = s[4];
				gender = s[5];
				yesNoValue = s[6];
				city = s[7];
				shelter = s[8];
				if(type.equals("Dog"))
				{
					dogBreeds.add(breed);
				}
				if(type.equals("Cat"))
				{
					catBreeds.add(breed);
				}
				if(type.equals("Bird"))
				{
					birdBreeds.add(breed);
				}
				if(yesNoValue.equals("yes"))
				{
					 conditional = "true";
				}
				else if(yesNoValue.equals("no"))
				{
					conditional = "false";
					System.out.println("not here");
				}
				else
				{
					conditional ="null";
				}
				Boolean b =Boolean.valueOf(conditional);
				Pet p = new Pet(type,name,age,breed,size,gender,b,city,shelters.findShelter(shelter));
				pets.add(p);
			}
		}
		catch(FileNotFoundException e)
		{
			System.out.println("The file does not exist");
		}
		catch(IOException e)
		{
			System.out.println("There was a problem with your input");
		}

	}
	// Feel free to add other helper methods as needed

}
