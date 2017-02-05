#import "state.h"

int readMode = 1;

void enableReadMode() {
	readMode = 1;
}

void disableReadMode() {
	readMode = 0;
}

int isReadModeEnabled() {
	return readMode;
}
