#include "DFUManager.h"

#include "BLEHandler.h"
#include "EslovHandler.h"

#if defined(ARDUINO_NICLA)
  SPIFBlockDevice DFUManager::_bd(SPI_PSELMOSI0, SPI_PSELMISO0,
                     SPI_PSELSCK0, CS_FLASH, 16000000);

mbed::LittleFileSystem DFUManager::_fs("fs");

#endif

#if defined(ARDUINO_NICLA)
DFUManager::DFUManager() :
  _target(NULL),
  _acknowledgment(DFUNack),
  _transferPending(false),
  _debug(NULL)
{
}
#else:
  DFUManager::DFUManager() {}
#endif

DFUManager::~DFUManager()
{
}

bool DFUManager::begin()
{
  #if defined(ARDUINO_NICLA)
  int err = _fs.mount(&_bd);
  if (err) {
    if(_debug) {
      _debug->print("Error mounting file system: ");
      _debug->println(err);
    }

    if (_fs.reformat(&_bd)) {
      if(_debug) {
        _debug->println("Error reformatting file system");
      }
      return false;
    }

  }
  return true;
  #endif
}

void DFUManager::processPacket(DFUSource source, DFUType dfuType, const uint8_t* data)
{
  #if defined(ARDUINO_NICLA)
  _transferPending = true;
  _dfuSource = source;

  DFUPacket* packet = (DFUPacket*)data;

  if (source == bleDFU) {
    //If Eslov is still active, turn it off
    if (eslovHandler.eslovActive) {
      eslovHandler.eslovActive = false;
      eslovHandler.end();
    }
  } else {
    //If BLE is still active, turn it off
    if (bleHandler.bleActive) {
      bleHandler.bleActive = false;
      bleHandler.end();
    }
  }

  if (_debug) {
    _debug->print("packet: ");
    _debug->println(packet->index);
  }

  if (packet->index == 0) {
    if (dfuType == DFU_INTERNAL) {
      _target = fopen("/fs/ANNA_UPDATE.BIN", "wb");
    } else {
      _target = fopen("/fs/BHY_UPDATE.BIN", "wb");
    }

    if(_debug) {
      bool target_found = (_target != NULL);
      _debug->print("target_found = ");
      _debug->println(target_found);
    }
  }

  if (_target != NULL) {
    int ret = fwrite(&packet->data, packet->last ? packet->remaining : sizeof(packet->data), 1, _target);
    // Set the acknowledgment flag according to the write return value
    if(_debug) {
      _debug->print("ret: ");
      _debug->println(ret);
    }
    if (ret > 0) _acknowledgment = DFUAck;
    else _acknowledgment = DFUNack;
  }

  if (packet->last) {
    if(_debug) {
      _debug->print("Last packet received. Remaining: ");
      _debug->println(packet->remaining);
    }
    fclose(_target);
    _target = NULL;
  }
  #endif
}

bool DFUManager::isPending()
{
  #if defined(ARDUINO_NICLA)
  return _transferPending;
  #endif
}

DFUSource DFUManager::dfuSource()
{
  #if defined(ARDUINO_NICLA)
  return _dfuSource;
  #endif
}

void DFUManager::closeDfu()
{
  #if defined(ARDUINO_NICLA)
  _transferPending = false;
  #endif
}

// acknowledgment flag is reset when read
uint8_t DFUManager::acknowledgment()
{
  #if defined(ARDUINO_NICLA)
  uint8_t ack = _acknowledgment;
  // Reset acknowledgment
  _acknowledgment = DFUNack;
  return ack;
  #endif
}

void DFUManager::debug(Stream &stream)
{
  #if defined(ARDUINO_NICLA)
  _debug = &stream;
  #endif
}

DFUManager dfuManager;
