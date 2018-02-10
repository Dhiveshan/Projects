package project4;
import java.awt.Dimension;
import java.io.File;
import java.util.Scanner;
import javax.swing.JFrame;
public class Driver {

	public static void main(String[] args) {
		
		SongList sL = new SongList();
		/*sL.insertByTitle(new Song("b","b",null));
		sL.insertByTitle(new Song("a","a",null));
		sL.insertByTitle(new Song("d","d",null));
		sL.insertByTitle(new Song("e","e",null));
		sL.insertByTitle(new Song("c","c",null));
		sL.insertByTitle(new Song("f","f",null));
		sL.printNodes();
		*/
		//File[] dir = new File("mp3").listFiles();
		//Scanner scanner = new Scanner(System.in);
		SongDatabase sD = new SongDatabase();
		//UserInterface ui = new UserInterface(dir);
		//sD.LoadSongs(dir);
		//System.out.println("Enter the keywords or name of a song");
		//String search  = scanner.nextLine();
		//sD.searchSong(search);
		//System.out.println("Enter the number of the song you would like to play");
		//int num = scanner.nextInt();
		//sD.playSong(num);
		
		JFrame frame = new JFrame("Mp3 player");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		// FILL IN CODE: create a SongCollection
		
		MPlayerPanel panel = new MPlayerPanel(sD); //Change this so that the constructor calls SongDatabase as a parameter.
		panel.setPreferredSize(new Dimension(600, 400));
		frame.add(panel);
		frame.pack();
		frame.setVisible(true);
		// TODO Auto-generated method stub
		 
		 
		
	}

}
