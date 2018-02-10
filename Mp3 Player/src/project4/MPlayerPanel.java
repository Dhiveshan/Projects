package project4;
/** The GUI for the Mp3 player. */
import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.Iterator;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;

@SuppressWarnings("serial")
public class MPlayerPanel extends JPanel {

	// FILL IN CODE: add an instance variable of type SongDatabase
	private SongDatabase songDB;
	// (or whatever you called this class in part 1)

	// GUI-related stuff
	private JPanel topPanel, bottomPanel;

	// Buttons in the top panel
	private JButton loadMp3Button, searchButton;
	private JTextField searchBox; // for searching by keyword
	private final JFileChooser fc = new JFileChooser();

	// Table in the central panel
	private JTable table = null;
	private JScrollPane centerPanel; // the panel that displays songs

	// Buttons in the bottom panel
	private JButton playButton, stopButton, exitButton;

	private int selectedSong = -1; // the id of the song that is selected in the
									// panel

	/**
	 * FILL IN CODE: Change this constructor so that it takes a SongDatabase (or
	 * whatever you called this class) as a parameter and assigns it to the
	 * corresponding instance variable.
	 */
	public MPlayerPanel(SongDatabase sd) {
		this.songDB = sd;
		this.setLayout(new BorderLayout());
		// Create panels: top, center, bottom
		topPanel = new JPanel();
		topPanel.setLayout(new GridLayout(1, 4));

		// create buttons
		loadMp3Button = new JButton("Load mp3");
		searchBox = new JTextField(10);
		searchButton = new JButton("Search");
		exitButton = new JButton("Exit");
		playButton = new JButton("Play");
		stopButton = new JButton("Stop");

		// add a listener for each button
		loadMp3Button.addActionListener(new MyButtonListener());
		searchButton.addActionListener(new MyButtonListener());
		playButton.addActionListener(new MyButtonListener());
		stopButton.addActionListener(new MyButtonListener());
		exitButton.addActionListener(new MyButtonListener());

		// add buttons and a textfield to the top panel
		topPanel.add(loadMp3Button);
		topPanel.add(searchBox);
		topPanel.add(searchButton);
		this.add(topPanel, BorderLayout.NORTH);

		// create the bottom panel and add three buttons to it
		bottomPanel = new JPanel();
		bottomPanel.setLayout(new GridLayout(1, 3));
		bottomPanel.add(playButton);
		bottomPanel.add(stopButton);
		bottomPanel.add(exitButton);

		this.add(bottomPanel, BorderLayout.SOUTH);

		// the panel in the center that shows mp3 songs
		centerPanel = new JScrollPane();
		this.add(centerPanel, BorderLayout.CENTER);

		// FileChooser: set the default directory to the current directory
		fc.setCurrentDirectory(new File("."));
	}

	/**
	 * Create a table that shows the title and the artist of each song in the
	 * center panel.
	 * 
	 * @param it
	 *            An iterator over Songs.
	 */
	public void displaySongs(Iterator<SongNode> it) {
		int count=0;
		int numSongs = songDB.getNumSongs(); // FILL IN CODE: change numSongs
		// so that it is the number of songs in the SongDatabase
		String[][] tableElems = new String[numSongs][2];
		String[] columnNames = { "Title", "Artist" };

		// FILL IN CODE:
		// Fill out tableElems array.
		// Each row of tableElems should contain the title and the artist
		// of a song (use the iterator to iterate over songs).
		while(it.hasNext())
		{
			Song s = it.next().elem();
			String title = s.getName();
			System.out.println(title);
			String artist = s.getAuthor();
			System.out.println(artist);
			tableElems[count][0]=title;
			tableElems[count][1]= artist;
			count++;
		}
		System.out.println(count);
		System.out.println(tableElems[0][1]);
		// the code below will add the table to the centerPanel
		table = new JTable(tableElems, columnNames);
		
		centerPanel.getViewport().add(table);
	}

	/** A inner listener class for buttons. **/
	class MyButtonListener implements ActionListener {

		public void actionPerformed(ActionEvent e) {
			if (e.getSource() == loadMp3Button) {
				System.out.println("Load mp3 button");
				// Allow the user to select a directory with mp3 files
				// FileChooser will open a separate window and will allow the
				// user
				// to select a directory
				fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
				fc.setDialogTitle("Select a directory with mp3 songs");
				int returnVal = fc.showOpenDialog(MPlayerPanel.this);
				if (returnVal == JFileChooser.APPROVE_OPTION) {
					File dir = fc.getSelectedFile(); // The directory selected
														// by the user
					// FILL IN CODE:
					// -find all mp3 songs in this directory and its
					File[] newDir = dir.listFiles();
					System.out.println(newDir[0].toString());
					songDB.LoadSongs(newDir);
					// subdirectories,
					// -extract tags,
					// -add them to the SongCollection, and display them in
					// central panel using the displaySongs method
					displaySongs(songDB.it());

				}
			} // else loadMp3 button
			else if (e.getSource() == playButton) {
				if (table == null)
					return;
				selectedSong = table.getSelectedRow(); // this tells you the id
														// of the
														// song that got
														// selected by
														// the user in the
				songDB.searchSong("");						// central panel
				// FILL IN CODE:
				songDB.playSong(selectedSong);
				
				//
				// Stop the current thread, and play the selected song
				// in a separate thread

			}

			// FILL IN CODE: add if statements for other buttons
			if(e.getSource() == stopButton)
			{
				selectedSong = table.getSelectedRow();
				songDB.stopSong(selectedSong);
			}
			if(e.getSource() == searchButton)
			{
				String songName = searchBox.getText();
				songDB.searchSong(songName);
				displaySongs(songDB.newIt());
			}
			updateUI();

		} // actionPerformed
	} // ButtonListener
}
