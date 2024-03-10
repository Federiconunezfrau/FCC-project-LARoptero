import re
import msg


def parse_data_file(fileHandler):

    msgsReceived = []

    for line in fileHandler:
        msgsReceived.append(msg.msgTimeStampService(line,timerBits=32))

    return msgsReceived