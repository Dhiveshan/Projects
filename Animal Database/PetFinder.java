/** The Driver class for the project. Interacts with the user. */
import java.util.Scanner;
public class PetFinder {

	public static void main(String[] args) {
		if (args.length < 2) {
			System.out.println("You did not provide the names of the files");
			return;
		}
		String filenameShelters = args[0]; // the name of the file with shelters
		String filenamePets = args[1]; // the name of the file with pets

		ShelterDatabase shDb = new ShelterDatabase(filenameShelters); 
		PetDatabase petDb = new PetDatabase(filenamePets, shDb); 
		
		// FILL IN CODE:
		// Create a Scanner and interact with the user, allowing them
		Scanner scanner = new Scanner(System.in);
		System.out.println("Do you want to search for a pet or a shelter?");
		String search = scanner.nextLine();
		if(search.equals("pet"))
		{
			System.out.println("Enter the type of pet: Cat, Dog or Bird or leave blank for any");
			String type = scanner.nextLine();
			System.out.println("The following breeds are available:");
			if(type.equals("Dog"))
			{
				System.out.println(petDb.getDogBreeds());
			}
			if(type.equals("Cat"))
			{
				System.out.println(petDb.getCatBreeds());
			}
			if(type.equals("Bird"))
			{
				System.out.println(petDb.getBirdBreeds());
			}
			System.out.println("Enter a breed");
			String breed = scanner.nextLine();
			if (breed.length() > 0)
			{
				 breed = " " + breed;
			}
			System.out.println("Enter age (young, adult, senior): ");
			String age = scanner.nextLine();
			if (age.length() > 0)
			{
				age = " " + age;
			}
			System.out.println("Enter size (small, medium, large): ");
			String size = scanner.nextLine();
			if (size.length() > 0)
			{
				size = " " + size;
			}
			System.out.println("Enter gender (male, female): ");
			String gender = scanner.nextLine();
			if (gender.length() > 0)
			{
				gender = " " + gender;
			}
			System.out.println("Is indoor " + type + "? (yes,no)");
			String yesNoValue = scanner.nextLine();
			if (yesNoValue.length() > 0)
			{
				yesNoValue = " " + yesNoValue;
			}
			System.out.println("Enter a city in CA: ");
			String city = scanner.nextLine();
			if (city.length() > 0)
			{
				city = " " + city;
			}
			petDb.displayPets(petDb.findPets(type,breed,age,size,gender,yesNoValue,city));
			//System.out.println(petDb.findPets(type,breed,age,size,gender,yesNoValue,city));
		}
		if(search.equals("shelter"))
		{
			System.out.println("Enter the city: ");
			String citySearch = scanner.nextLine();
			if(citySearch.length()>0)
			{
				citySearch = " " + citySearch;
			}
			System.out.println("Enter a keyword from the name of the shelter: ");
			String keyWord = scanner.nextLine();
			System.out.println("Enter the zipcode if you know it: ");
			String zipcode = scanner.nextLine();
			if(zipcode.length()>0)
			{
				zipcode = " " + zipcode;
			}
			System.out.println(shDb.findShelters(citySearch,keyWord,zipcode));
		}
		// to search for pets or shelters using certain search criteria
		
		
	}
	
	
	

}