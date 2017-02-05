#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#define MESSAGE_LENGTH 60

// PUBLIC
void sendMessage(char *message);

// PRIVATE
void sendPendingMessages();
int hasMessagesInQueue();
int addMessageToQueue(char *message);
int getMessageFromQueue(char **message);

#endif
