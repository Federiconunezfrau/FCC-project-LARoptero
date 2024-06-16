#ifndef _IHANDLER_HPP_
#define _IHANDLER_HPP_

#include <stdint.h>

enum class COMM_MODE{
    BLCK    =   0,
    IT          ,
    DMA         ,
};

enum class iPHANDLER_ST{
    OK              =  0,
    ERROR               ,
    UNAVAILABLE         ,
    INVALID             ,
//    HANDLER_UNAVAILABLE ,
};



/** Pensar si conviene usar esta estructura como parámetro de las funciones
 * Los que usan esta interface, crean este struct y se lo pasan por copia a
 * read_registers y write_registers. Se logran 2 cosas con esto
 * Si más adelante se quieren pasar más opciones a las funciones se puede
 * hacer y si se quiere que las propias funciones read_registers y write_registers
 * verifiquen si está dispoinible el periférico para enviar el datos se puede hacer
 * **/
struct PHandlerOpt {
    uint8_t     add;
    uint8_t     size;
    void        *slave  =   nullptr;
    COMM_MODE   mode    =   COMM_MODE::BLCK;
    bool        rw      =   true;   // se le pasa al parámetro normalMode de add_read_bit y add_write_bit
    bool 		rwFront = 	true;
    uint8_t     addSize =   1;      // Tamaño en bytes de la dirección del periférico.
};


class iPHandler {
    public:

    uint8_t *m_recvBuffer;
    //virtual iHANDLER_ST read_registers(uint8_t add, uint8_t size, void *slave, COMM_MODE mode = COMM_MODE::BLCK);
    //virtual iHANDLER_ST write_registers(uint8_t add, uint8_t *data, uint8_t size, void *slave, COMM_MODE mode = COMM_MODE::BLCK);


    virtual iPHANDLER_ST read_registers(const PHandlerOpt &opt) = 0;
    virtual iPHANDLER_ST write_registers(uint8_t *data, const PHandlerOpt &opt) = 0;


    static uint8_t add_read_bit(uint8_t reg, bool normalMode = true, bool rwBitInFront = true)
    {
    	if(rwBitInFront){
    		return normalMode ? (reg | (1 << 7)) : reg;
    	}else{
    		return normalMode ? (reg << 1) | 0x01 : reg << 1;
    	}
    }

    static uint8_t add_write_bit(uint8_t reg, bool normalMode = true, bool rwBitInFront = true)
    {
    	if(rwBitInFront){
    		return normalMode ? reg : (reg | (1 << 7));
    	}else{
    		return normalMode ? reg << 1 : (reg  << 0x01 | 1);
    	}
    }

    bool lock()
	{
		if(m_mutex){
			return false;
		}
		m_mutex = true;
		return true;
	}


	bool unlock()
	{
		if(!m_mutex){
			return false;
		}
		m_mutex = false;
		return true;
	}

protected:
    bool 	m_mutex;


};

#endif
