# OCR SUDOKU

## How to use our application?

1. Open a terminal
2. Open the ocr_sudoku directory
3. Run the command "make gui"
4. Run the resulting executable ./gui

Now, the application will be open on the *Welcome* page.
1. Use the file chooser to select the image of the sudoke to solve
from your computer. It will then be displayed.
2. Press on the 'process' button at the bottom right in order to process the
chosen image.

The *Process* page will now open. On it, the user can see the processed image.
Press on the 'check' button to launch the detection of the digits.

The *Identify Digits* page will now open. On it, two images will be displayed :
1. the processed image 
2. the newly generated image with the digits recognized by the neural network.
By pressing on 'solve', the user launches the sudoku solving algorithm.

The *Solve* page will now open. Displayed is the newly generated solved
sudoku grid.
There are two buttons.
1. Pressing the 'restart' button will reset everything, and take the user back	to the *Welcome* page.
2. Pressing the 'exit' button will close the application.
