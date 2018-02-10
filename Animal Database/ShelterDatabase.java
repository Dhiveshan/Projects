/** A class that represents a "Shelter Database". Contains a list of 
 * animal shelters.
 */
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.io.IOException;

public class ShelterDatabase {
	private ArrayList<AnimalShelter> shelters;

	/** Constructor. Calls readShelters */
	public ShelterDatabase(String filename) {
		shelters = new ArrayList<AnimalShelter>();
		readShelters(filename);
	}

	/** Add a shelter to the list of shelters */
	public void addShelter(AnimalShelter a) {
		// FILL IN CODE
		shelters.add(a);

	}

	/** Read shelters from the given file, and add them to the ArrayList
	 * shelters defined above. */
	public void readShelters(String filename) {
		// FILL IN CODE
		try
		{
		
		FileReader shelterFile = new FileReader(filename);
		BufferedReader br = new BufferedReader(shelterFile);
		String line;
		while((line = br.readLine()) != null)
		{
			String[] s;
			String shelterName;
			String street;
			String city;
			String state;
			String zipcode;
			String phone;
			String description;
			String[] address;
			s = line.split(";");
			address =s[1].split(",");
			shelterName = s[0];
			street = address[0];
			city = address[1];
			state = address[2];
			zipcode = address[3];
			phone = s[2];
			description = s[3];
			Address a = new Address(street,city,zipcode);
			AnimalShelter as = new AnimalShelter(shelterName,a,phone,description);
			shelters.add(as);
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

	/** Find an animal shelter based on the name */
	public AnimalShelter findShelter(String name) {
		// FILL IN CODE
		// iterate over the ArrayList and find a shelter with the given name
		for(int i =0; i<shelters.size(); i++)
		{
			if(name.contains(shelters.get(i).getName()))
			{
				return shelters.get(i);
			}
		}
		return null; // change
	}

	/** Find shelters according to the specified parameters */
	public ArrayList<AnimalShelter> findShelters(String city, String keyword, String zipcode) {
		// FILL IN CODE
		ArrayList<AnimalShelter> newList = new ArrayList<AnimalShelter>();
		for(int i =0; i<shelters.size();i++)
		{
			AnimalShelter s = shelters.get(i);
			Address a = s.getAddress();
			String c = a.getCity();	
			if(city.contains(c) || city.equals(""))
			{

				if(shelters.get(i).getDescription().contains(keyword) || keyword.equals(""))
				
					if(zipcode.equals(shelters.get(i).getAddress().getZipCode()) || zipcode.equals(""))
					{
						newList.add(shelters.get(i));
					}
				
			}
		}
		
		return newList; // change
	}
	
	// Feel free to add helper methods as needed
}
