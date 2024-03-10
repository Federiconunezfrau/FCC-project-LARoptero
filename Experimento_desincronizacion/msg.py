import re
import utils

class msgTimeStampService:
    __pattern_cmd = re.compile(r'cmd:(.+?) \|')
    __pattern_lenPayload = re.compile(r'len:(.+?) \|')
    __pattern_payload = re.compile(r'payload:(.+)')

    __shift_node_id = 32

    __shift_timeStampCount = 16
    __mask_timeStampCount   = 0x00000000FFFFFFFF0000

    __mask_timeStampCounter = 0x0000000000000000FFFF

    __mask_timeStampCounter_32 = 0x00000000FFFFFFFF

    def __init__(self, byteString, timerBits=16):

        self.m_cmd = re.search(self.__pattern_cmd, byteString).group(1)
        self.m_len = re.search(self.__pattern_lenPayload, byteString).group(1)

        payloadString = re.search(self.__pattern_payload, byteString).group(1)

        self.m_payload = int(payloadString.replace(' ',''),16)

        # Se obtiene el número de ID del mensaje
        self.m_nodeID = utils.flip_bytes(self.m_payload>>self.__shift_node_id, 4,"big")

        if(timerBits==16):

            # Se obtienen los segundos
            timeStampCount = utils.flip_bytes((self.m_payload & self.__mask_timeStampCount)>>self.__shift_timeStampCount,4,"big")
            timeStampCounter = utils.flip_bytes(self.m_payload & self.__mask_timeStampCounter,2,"big")
            self.m_seconds = (timeStampCount * 50000 + timeStampCounter) * 1e-6
        
        if(timerBits==32):

            # Se obtienen los segundos
            timeStampCounter = utils.flip_bytes(self.m_payload & self.__mask_timeStampCounter_32,4,"big")
            self.m_useconds = timeStampCounter