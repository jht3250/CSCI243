/// 
/// File: triangle.c 
/// 
/// A program to print out multiple triangles of the form:
/// <pre>
///    *
///  ***
/// ****
/// </pre>
/// 
/// @author jht3250 : John Treon
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO_ADD_#INCLUDES_ HERE
#include <stdio.h>

/**
 *  Function: drawTriangle
 *
 *  Description: Draw a triangle whose base width is 'size'.
 *  Note: If size is even, the function makes a 'size + 1' triangle. 
 * 
 *  @param size  the width of the base of the triangle to draw
 */
// TODO_WRITE_DRAWTRIANGLE_FUNCTION HERE
void drawTriangle(int size) {
	int finalSize = size;
	if (finalSize % 2 == 0) {
		finalSize++;
	}
	for (int stars = 1; stars <= finalSize; stars = stars + 2) {
		for (int draw = 1; draw < finalSize; draw++) {
			if (draw >= finalSize - stars) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
}
/**
 *  Function: main 
 *
 *  Description: draws pyramid triangles of size 1, 5, and 6.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
// TODO_WRITE_MAIN_FUNCTION HERE
int main() {
	drawTriangle(1);
	printf("\n");
	drawTriangle(5);
	printf("\n");
	drawTriangle(6);
	printf("\n");

	return 0;	
}

