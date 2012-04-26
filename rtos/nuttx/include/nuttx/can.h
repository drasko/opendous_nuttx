/************************************************************************************
 * include/nuttx/can.h
 *
 *   Copyright (C) 2008, 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __NUTTX_CAN_H
#define __NUTTX_CAN_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <nuttx/fs.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* Default configuration settings that may be overridden in the board configuration.
 * file.  The configured size is limited to 255 to fit into a uint8_t.
 */

#if !defined(CONFIG_CAN_FIFOSIZE)
#  define CONFIG_CAN_FIFOSIZE 8
#elif CONFIG_CAN_FIFOSIZE > 255
#  undef  CONFIG_CAN_FIFOSIZE
#  define CONFIG_CAN_FIFOSIZE 255
#endif

#if !defined(CONFIG_CAN_NPENDINGRTR)
#  define CONFIG_CAN_NPENDINGRTR 4
#elif CONFIG_CAN_NPENDINGRTR > 255
#  undef  CONFIG_CAN_NPENDINGRTR
#  define CONFIG_CAN_NPENDINGRTR 255
#endif

/* Convenience macros */

#define dev_reset(dev)            dev->cd_ops->co_reset(dev)
#define dev_setup(dev)            dev->cd_ops->co_setup(dev)
#define dev_shutdown(dev)         dev->cd_ops->co_shutdown(dev)
#define dev_txint(dev,enable)     dev->cd_ops->co_txint(dev,enable)
#define dev_rxint(dev,enable)     dev->cd_ops->co_rxint(dev,enable)
#define dev_ioctl(dev,cmd,arg)    dev->cd_ops->co_ioctl(dev,cmd,arg)
#define dev_remoterequest(dev,id) dev->cd_ops->co_remoterequest(dev,id)
#define dev_send(dev,m)           dev->cd_ops->co_send(dev,m)
#define dev_txempty(dev)          dev->cd_ops->co_txempty(dev)

/* CAN message support */

#define CAN_MAXDATALEN 8
#define CAN_ID(hdr)               ((uint16_t)(hdr) >> 5)
#define CAN_RTR(hdr)              (((hdr) & 0x0010) != 0)
#define CAN_DLC(hdr)              ((hdr) & 0x0f)
#define CAN_MSGLEN(hdr)           (sizeof(struct can_msg_s) - (CAN_MAXDATALEN - CAN_DLC(hdr)))

/* Built-in ioctl commands
 *
 * CANIOCTL_RTR: Send the remote transmission request and wait for the response.
 */

#define CANIOCTL_RTR              1 /* Argument is a reference to struct canioctl_rtr_s */

/* CANIOCTL_USER: Device specific ioctl calls can be supported with cmds greater
 * than this value
 */

#define CANIOCTL_USER             2

/************************************************************************************
 * Public Types
 ************************************************************************************/
/* CAN-message Format
 *
 * One CAN-message consists of a maximum of 10 bytes.  A message is composed of at
 * least the first 2 bytes (when there are no data bytes).
 *
 * Byte 0:      Bits 0-7: Bits 3-10 of the 11-bit CAN identifier
 * Byte 1:      Bits 5-7: Bits 0-2 of the 11-bit CAN identifier
 *              Bit 4:    Remote Tranmission Request (RTR)
 *              Bits 0-3: Data Length Code (DLC)
 * Bytes 2-10: CAN data
 *
 * The struct can_msg_s holds this information in a user-friendly, unpacked form.
 * This is the form that is used at the read() and write() driver interfaces.  The
 * message structure is actually variable length -- the true length is given by
 * the CAN_MSGLEN macro.
 */

struct can_msg_s
{
  uint16_t      cm_hdr;                  /* The 16-bit CAN header */
  uint8_t       cm_data[CAN_MAXDATALEN]; /* CAN message data (0-8 byte) */
};

/* This structure defines a CAN message FIFO. */

struct can_fifo_s
{
  sem_t         cf_sem;                  /* Counting semaphore */
  uint8_t       cf_head;                 /* Index to the head [IN] index in the circular buffer */
  uint8_t       cf_tail;                 /* Index to the tail [OUT] index in the circular buffer */
                                         /* Circular buffer of CAN messages */
  struct can_msg_s cf_buffer[CONFIG_CAN_FIFOSIZE];
};

/* The following structure define the logic to handle one RTR message transaction */

struct can_rtrwait_s
{
  sem_t         cr_sem;                  /* Wait for RTR response */
  uint16_t      cr_id;                   /* The ID that is waited for */
  FAR struct can_msg_s *cr_msg;          /* This is where the RTR reponse goes */
};

/* This structure defines all of the operations providd by the architecture specific
 * logic.  All fields must be provided with non-NULL function pointers by the
 * caller of can_register().
 */

struct can_dev_s;
struct can_ops_s
{
  /* Reset the CAN device.  Called early to initialize the hardware. This
   * is called, before co_setup() and on error conditions.
   */

  CODE void (*co_reset)(FAR struct can_dev_s *dev);

  /* Configure the CAN. This method is called the first time that the CAN
   * device is opened.  This will occur when the port is first opened. 
   * This setup includes configuring and attaching CAN interrupts.  Interrupts
   * are all disabled upon return.
   */

  CODE int (*co_setup)(FAR struct can_dev_s *dev);

  /* Disable the CAN.  This method is called when the CAN device is closed.
   * This method reverses the operation the setup method.
   */

  CODE void (*co_shutdown)(FAR struct can_dev_s *dev);

  /* Call to enable or disable RX interrupts */

  CODE void (*co_rxint)(FAR struct can_dev_s *dev, bool enable);

  /* Call to enable or disable TX interrupts */

  CODE void (*co_txint)(FAR struct can_dev_s *dev, bool enable);

  /* All ioctl calls will be routed through this method */

  CODE int (*co_ioctl)(FAR struct can_dev_s *dev, int cmd, unsigned long arg);

  /* Send a remote request */

  CODE int (*co_remoterequest)(FAR struct can_dev_s *dev, uint16_t id);

  /* This method will send one message on the CAN */

  CODE int (*co_send)(FAR struct can_dev_s *dev, FAR struct can_msg_s *msg);

  /* Return true if all message have been sent.  If for example, the CAN
   * hardware implements FIFOs, then this would mean the transmit FIFO is
   * empty.  This method is called when the driver needs to make sure that
   * all characters are "drained" from the TX hardware before calling co_shutdown().
   */

  CODE bool (*co_txempty)(FAR struct can_dev_s *dev);
};

/* This is the device structure used by the driver.  The caller of
 * can_register() must allocate and initialize this structure.  The
 * calling logic need only set all fields to zero except:
 *
 *   The elements of 'cd_ops', and 'cd_priv'
 *
 * The common logic will initialize all semaphores.
 */

struct can_dev_s
{
  uint8_t              cd_ocount;        /* The number of times the device has been opened */
  uint8_t              cd_npendrtr;      /* Number of pending RTR messages */
  uint8_t              cd_ntxwaiters;    /* Number of threads waiting to enqueue a message */
  sem_t                cd_closesem;      /* Locks out new opens while close is in progress */
  sem_t                cd_recvsem;       /* Used to wakeup user waiting for space in cd_recv.buffer */
  struct can_fifo_s    cd_xmit;          /* Describes transmit FIFO */
  struct can_fifo_s    cd_recv;          /* Describes receive FIFO */
                                         /* List of pending RTR requests */
  struct can_rtrwait_s cd_rtr[CONFIG_CAN_NPENDINGRTR];
  FAR const struct can_ops_s *cd_ops;    /* Arch-specific operations */
  FAR void            *cd_priv;          /* Used by the arch-specific logic */
};

/* Structures used with ioctl calls */

struct canioctl_rtr_s
{
  uint16_t              ci_id;           /* The 11-bit ID to use in the RTR message */
  FAR struct can_msg_s *ci_msg;          /* The location to return the RTR response */
};

/************************************************************************************
 * Public Data
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Name: can_register
 *
 * Description:
 *   Register a CAN driver.
 *
 ************************************************************************************/

EXTERN int can_register(FAR const char *path, FAR struct can_dev_s *dev);

/************************************************************************************
 * Name: can_receive
 *
 * Description:
 *   Called from the CAN interrupt handler when new read data is available
 *
 * Parameters:
 *   dev  - The specifi CAN device
 *   hdr  - The 16-bit CAN header
 *   data - An array contain the CAN data.
 *
 * Return:
 *   OK on success; a negated errno on failure.
 *
 ************************************************************************************/

EXTERN int can_receive(FAR struct can_dev_s *dev, uint16_t hdr, FAR uint8_t *data);

/************************************************************************************
 * Name: can_txdone
 *
 * Description:
 *   Called from the CAN interrupt handler at the completion of a send operation.
 *
 * Parameters:
 *   dev  - The specifi CAN device
 *   hdr  - The 16-bit CAN header
 *   data - An array contain the CAN data.
 *
 * Return:
 *   OK on success; a negated errno on failure.
 *
 ************************************************************************************/

EXTERN int can_txdone(FAR struct can_dev_s *dev);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __NUTTX_CAN_H */
