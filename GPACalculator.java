/* GUI GPA Calculator
 * Adam Marcus aem2nj
 * This is a GUI GPA Calculator Applet made using Java
 *
 * Sources:
 * https://docs.oracle.com/javase/tutorial/uiswing/components/table.html
 * https://docs.oracle.com/javase/8/docs/api/javax/swing/JTextField.html#setHorizontalAlignment-int-
 * https://docs.oracle.com/javase/tutorial/uiswing/layout/gridbag.html
 * https://docs.oracle.com/javase/8/docs/api/java/awt/GridBagConstraints.html
 * http://www.java2s.com/Tutorial/Java/0240__Swing/CreatingaJTable.htm
 * https://stackoverflow.com/questions/8658175/text-align-in-jlabel
 * https://docs.oracle.com/javase/7/docs/api/javax/swing/JButton.html
 * https://stackoverflow.com/questions/3549206/how-to-add-row-in-jtable
 * https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/parseInt
 * https://www.cs.cmu.edu/~mrmiller/15-110/Handouts/arrays2D.pdf
 * https://stackoverflow.com/questions/5710394/how-do-i-round-a-double-to-two-decimal-places-in-java
 * https://www.youtube.com/watch?v=ytV80z6XgPw
 * https://stackoverflow.com/questions/1090098/newline-in-jlabel
 * 
 */

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DecimalFormat;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableModel;

import java.applet.Applet;

 public class GPACalculator extends Applet {
	 
	 //These are the initial fields to make the JTable I used
	 Object[] columnNames = new Object[]{"Class", "GPA", "Credit Hours"};
	 Object[][] tableRows = new Object[][]{{"Course Name","GPA", "Credit Hours"}};
	 
	 //These are necessary for any GridBag Layout.  I got them from the GridBagLayout api
     GridBagLayout gridbag = new GridBagLayout();
     GridBagConstraints c = new GridBagConstraints();
     
     //Initial fields that I need for calculations
     private double gpa;
     private double totalCredits;
     private double gradePoints;
     
     //A string array with all of the valid  GPA's someone could input when adding a course
     final String[] possibleGPAs = new String[] {"0.0","0", "0.3", "0.7","1.0","1", "1.3", "1.7", "2.0","2", "2.3", "2.7","3.0","3", "3.3", "3.7","4.0","4"};
     
     //JButtons to add individual courses, 15 credits, calculate things, remove individual courses, or remove all courses and reset the calculator
     JButton addCourse = new JButton("Add Course");
     JButton add15 = new JButton("Add 15 Blank Credits");
     JButton calculate = new JButton("Calculate");
     JButton removeRow = new JButton("Remove Selected Course");
     JButton removeAll = new JButton("Remove All Courses");
     
     //JTextFields where the user can input course names, gpa, credit hours, and their target GPA
     JTextField courseName = new JTextField("Enter Course Name (optional)");
     JTextField gpaInput = new JTextField("Enter GPA (optional)");
     JTextField hoursInput = new JTextField("Credit Hours");
     JTextField targetGPA = new JTextField("4.0");
     
     //JTable storing all of the courses that have been entered
     JTable table = new JTable(new DefaultTableModel(tableRows, columnNames));
     //Necessary to do any calculations with the table values
     DefaultTableModel model = (DefaultTableModel) table.getModel();
     
     //Basically glorified text boxes that I can change but not the user
     //Displays information I provide for them
     JLabel targetGPALabel = new JLabel("Target GPA: ");
     JLabel currentGPA = new JLabel("Current GPA: "+gpa);
     JLabel recommendation = new JLabel("Recommendation: ");
     
     
     public void init() {//the ratio of comments to code is not that high for this section because there is so much repeated

         setFont(new Font("SansSerif", Font.PLAIN, 20));
         setLayout(gridbag);

         c.fill = GridBagConstraints.BOTH;

         //Any use of c.weightx or c.weighty lets determines how much of the row or column each component takes up as the size changes
         c.weightx = 0.5;
         c.weighty = 0.3;
         
         //All of these little blocks are used to add each component to the GridBag
         gridbag.setConstraints(courseName,c);
    	 courseName.setHorizontalAlignment(JTextField.LEFT);//changes where the text is aligned in the box
    	 add(courseName);
    	 
    	 c.weightx = 0.5;
    	 
         gridbag.setConstraints(gpaInput,c);
    	 gpaInput.setHorizontalAlignment(JTextField.LEFT);
    	 add(gpaInput);
    	 
    	 gridbag.setConstraints(hoursInput,c);
    	 hoursInput.setHorizontalAlignment(JTextField.LEFT);
    	 add(hoursInput);

    	//This next line means that the component right after it is the last component on that row
         c.gridwidth = GridBagConstraints.REMAINDER;
         gridbag.setConstraints(addCourse, c);
         add(addCourse);
         
         c.weightx = 0.0;//0.0 is default for weight
         gridbag.setConstraints(add15, c);
         add(add15);
         
         c.gridwidth = 1;
         c.weightx = 0.3;
         
         targetGPALabel.setHorizontalAlignment(SwingConstants.CENTER);
    	 gridbag.setConstraints(targetGPALabel, c);
    	 add(targetGPALabel);
    	 
         c.weightx = 1.0;
         
         gridbag.setConstraints(targetGPA,c);
    	 targetGPA.setHorizontalAlignment(JTextField.LEFT);
    	 add(targetGPA);
    	 
    	 gridbag.setConstraints(calculate, c);
    	 calculate.setHorizontalAlignment(SwingConstants.CENTER);
    	 add(calculate);
    	 
         c.gridwidth = GridBagConstraints.REMAINDER;
         c.weightx = 0.3;

         currentGPA.setHorizontalAlignment(SwingConstants.CENTER);
    	 gridbag.setConstraints(currentGPA, c);
    	 add(currentGPA);

         c.gridwidth = 2;
         c.gridheight = 1;
         c.weighty = 1.0;
         c.weightx = 1.0;
         
         gridbag.setConstraints(table, c);
    	 add(table);

         c.gridwidth = GridBagConstraints.REMAINDER;
         c.gridheight = 1;
         c.weightx = 1.0;
         c.weighty = 0.3;
         
         recommendation.setHorizontalAlignment(SwingConstants.CENTER);
         recommendation.setVerticalAlignment(SwingConstants.NORTH);
    	 gridbag.setConstraints(recommendation, c);
    	 add(recommendation);
    	 
    	 c.gridheight = 1;
         c.gridwidth = 2;
    	 c.weightx = 0.5;
         c.weighty = 0.3;
         
         gridbag.setConstraints(removeRow, c);
    	 removeRow.setHorizontalAlignment(SwingConstants.CENTER);
    	 add(removeRow);

    	 c.gridwidth = GridBagConstraints.REMAINDER;
         gridbag.setConstraints(removeAll, c);
    	 removeAll.setHorizontalAlignment(SwingConstants.CENTER);
    	 add(removeAll);

    	 //These each assign their specified button the ActionListener written for them
    	 //Basically this just applies the listener method to each button
         addCourse.addActionListener(new addCourseButtonListener());
         add15.addActionListener(new add15ButtonListener());
         calculate.addActionListener(new calculateButtonListener());
         removeRow.addActionListener(new removeRowButtonListener());
         removeAll.addActionListener(new removeAllButtonListener());
         
         //Sets the initial size, but size can still be changed easily
         setSize(700, 400);
     }
     
     private class removeAllButtonListener implements ActionListener{
    	 //written specifically for the removeAll button
    	 public void actionPerformed(ActionEvent a) {
    		 while(model.getRowCount()>1) {//since the first row is the headers, it doesn't loop through that one
    			 model.removeRow(model.getRowCount()-1);
    		 }
    		 gpa = 0.0;//clears whatever the gpa was before
    		 currentGPA.setText("Current GPA: "+gpa);//resets the currentGPA text
    		 recommendation.setText("Recommendation: ");//resets the recommendation
    	 }
     }
     
     private class removeRowButtonListener implements ActionListener{
    	 //written specifically for the removeRow button
    	 public void actionPerformed(ActionEvent a) {
    		 //gets the number of the row they have selected
    		 int selectedRowIndex = table.getSelectedRow();
    		 //removes that row
    		 model.removeRow(selectedRowIndex);
    		 
    		 //these need to be reset each time before they're calculated
    		 gradePoints = 0;
  			 totalCredits = 0;
  			 for(int i = 1;i<model.getRowCount();i++) {
  				 //loops through each row
     			if(!model.getValueAt(i, 1).toString().equals("")) {
     				//checks so it only includes courses that have a gpa input already before calculating currentGPA
     				//then takes the values of each course's gpa and credit hours to calculate the current GPA
     				Double grade = Double.parseDouble(model.getValueAt(i, 1).toString());
     				Double credits = Double.parseDouble(model.getValueAt(i, 2).toString());
     				gradePoints += grade*credits;
     				totalCredits += credits;
     				gpa = gradePoints/totalCredits;
     			}
  			}
  			//uses a helper method to round the gpa to two places before displaying it
     		gpa = roundTwoDecimals(gpa);
     		currentGPA.setText("Current GPA: "+gpa);
     		//calls a helper method I wrote to calculate the gpa needed to reach their target gpa and update the recommendations label
     		calculateNeededGPA();
    	 }
     }
     
     private class add15ButtonListener implements ActionListener{
    	 //written specifically for the add 15 blank credits button
    	 public void actionPerformed(ActionEvent a) {
    		 //sets the name and gpa to blanks and the default credits to 3
    		 String nameInput = "";
    		 String readGPA = "";
    		 String readHours = "3";
    		 for(int i=0;i<5;i++) {
    			 //adds 5 of the blank 3 credit courses for a total of 15 blank credits added
     			 model.addRow(new Object[] {nameInput,readGPA,readHours}); 
    		 }
    	 }
     }
     
     private class calculateButtonListener implements ActionListener{
    	 //written for the calculate button
    	 //this one is a more general and versatile button
    	 public void actionPerformed(ActionEvent a) {
    		//calculates the current gpa whenever clicked, so you can edit values in the JTable then click calculate to update the others
    		//without having to add a blank class or anything
    		gradePoints = 0;
 			totalCredits = 0;
 			//calculates gpa
 			for(int i = 1;i<model.getRowCount();i++) {
    			if(!model.getValueAt(i, 1).toString().equals("")) {
    				Double grade = Double.parseDouble(model.getValueAt(i, 1).toString());
    				Double credits = Double.parseDouble(model.getValueAt(i, 2).toString());
    				gradePoints += grade*credits;
    				totalCredits += credits;
    				gpa = gradePoints/totalCredits;
    			}
 			}
    		gpa = roundTwoDecimals(gpa);
    		currentGPA.setText("Current GPA: "+gpa);
    		
    		//calls the same helper method as previously mentioned
    		calculateNeededGPA();
    	 }
     }
     
     private class addCourseButtonListener implements ActionListener {
    	 //written for the add course button
    	 public void actionPerformed(ActionEvent a) {
    		//initializes the name and gpa as blank
    		String nameInput = "";
    		String readGPA = "";
    		//initializes the credit hours as whatever was input in the credits input box
    		String readHours = hoursInput.getText();
    		
    		//sets the course name was whatever was in the box unless it is unchanged from its default
    		if(!courseName.getText().equals("Enter Course Name (optional)")) {
    			nameInput = courseName.getText();
    		}
    		
    		//checks to make sure the gpa that they put in is valid by looping through the possible gpa's array made at the beginning
    		boolean isGPA = false;
    		for(String s: possibleGPAs) {
    			if(s.equals(gpaInput.getText())) {
    				isGPA = true;
    			}
    		}
    		//if the gpa they input was valid then the gpa will become that rather than an empty string
    		if(isGPA){
    			readGPA = gpaInput.getText();
    		}
    		
    		
    		try {
    			//checks to make sure that the number of credit hours is valid (integer>=0)
	    		if(Integer.parseInt(hoursInput.getText())>=0) {
	    			//if it is valid, it adds that row to the table
	    			model.addRow(new Object[] {nameInput,readGPA,readHours});
	    			
	    			//then resets the input boxes
	        		courseName.setText("Enter Course Name (optional)");
	        		gpaInput.setText("Enter GPA (optional)");
	        		hoursInput.setText("Credit Hours");
	    		}else {
	    			//if the credit hours input was an integer but was not >= 0 then it resets that box and asks for them to input an integer > 0
	    			hoursInput.setText("Enter an integer > 0");
	    		}
    		}catch(NumberFormatException e){
    			//if their input for the credit hours was not an integer it will hit this and then ask them to put in an integer like in the else statement above it
    			hoursInput.setText("Enter an integer > 0");
    		}
    		
    		finally{
    			//then recalculates gpa
    			gradePoints = 0;
    			totalCredits = 0;
    			for(int i = 1;i<model.getRowCount();i++) {
	    			if(!model.getValueAt(i, 1).toString().equals("")) {
	    				Double grade = Double.parseDouble(model.getValueAt(i, 1).toString());
	    				Double credits = Double.parseDouble(model.getValueAt(i, 2).toString());
	    				gradePoints += grade*credits;
	    				totalCredits += credits;
	    				gpa = gradePoints/totalCredits;
	    			}
    			}
    		
    			//rounds and updates the current gpa
	    		gpa = roundTwoDecimals(gpa);
	    		currentGPA.setText("Current GPA: "+gpa);
	    		
	    		//calls the same helper method as the previous button listeners
	    		calculateNeededGPA();
    		}
    	 }
     }

     //this helper method I found online.  it simply takes in a double and rounds it to two decimal places
     //https://stackoverflow.com/questions/5710394/how-do-i-round-a-double-to-two-decimal-places-in-java
     public double roundTwoDecimals(double d) {
 	    DecimalFormat twoDForm = new DecimalFormat("#.##");
 	    return Double.valueOf(twoDForm.format(d));
     }
     
     //this helper method I wrote. it calculates the gpa they would need out of blank credits to hit their target GPA
     //it also updates the recommendation component based on the gpa they need
     public void calculateNeededGPA() {
    	 double potentialCredits = 0;
    	 //adds up the total number of credits in the table
		 for(int i = 1;i<model.getRowCount();i++) {
			potentialCredits += Double.parseDouble(model.getValueAt(i, 2).toString());
		 }
		 
		 //gets their desired gpa from the input box
		 double desiredGPA = Double.parseDouble(targetGPA.getText());
		 
		 //calculates the gpa they would need from each of their blank credits to hit their target gpa
		 double requiredGPAForGoal = (desiredGPA*potentialCredits-gradePoints)/(potentialCredits-totalCredits);
		 requiredGPAForGoal = roundTwoDecimals(requiredGPAForGoal);
		 
		 //if they need more than a 4.0 per blank course it tells them that and suggests adding more credits
		 if(requiredGPAForGoal>4.0) {
			 recommendation.setText("<html>Recommendation: Unfortunately you would need a GPA of "+requiredGPAForGoal+",</br> which is unattainable, to meet your target GPA.</br> Try adding more credit hours and recalculating.</html>");
		 }
		 //if they need less than a 2.0 per blank course it tells them that and says they can take fewer credits if they want
		 else if(requiredGPAForGoal<2.0) {
			 recommendation.setText("<html>Recommendation: You would only need a GPA of "+requiredGPAForGoal+" to meet your goal.</br> You could take fewer credit hours if you wish.</html>");
		 }
		 //otherwise it just tells them the gpa they would need from the blank courses
		 else {
			 recommendation.setText("Recommendation: You would need a GPA of "+requiredGPAForGoal+" to meet your goal.");
		 }
     }
     
     //the main method just creates the GPA Calculator when it is run
     //I got most of this part from the GridBagLayout api
     //https://docs.oracle.com/javase/tutorial/uiswing/layout/gridbag.html
     public static void main(String args[]) {
         Frame f = new Frame("GPACalculator");
         GPACalculator c = new GPACalculator();

         c.init();

         f.add("Center", c);
         f.pack();
         f.setSize(f.getPreferredSize());
     }
 }
