/*
 * LeptonAppTask.c
 *
 *  Created on: 2021. 6. 21.
 *      Author: Jitae
 */

#include "LeptonAppDriver.h"

osEventFlagsId_t lepton_id;

void vLeptonTask(void *argument)
{
  uint32_t debug = 0;

  if ((lepton_id = osEventFlagsNew(NULL)) == NULL)
  {
    /* TODO : Error 처리 */
    printf(" ERROR ## \n");
  }

  /* TODO : Lepton Init 구현 */

  for (;;)
  {
    /* TODO :  Check Wait Video Frame Request Signal */
    if ((debug = osEventFlagsWait(lepton_id, VIDEO_REQ_SIGNAL, osFlagsWaitAny, 1000)) == VIDEO_REQ_SIGNAL)
    {
      /* TODO : Message 처리 */
      printf("Recv VIDEO Req Sig \n");

    }

    /* TODO : Wait Lepton Configuration Request Signal */

    if ((debug =osEventFlagsWait(lepton_id, LEPTON_CONFIG_SIGNAL, osFlagsWaitAny, 1000)) == LEPTON_CONFIG_SIGNAL)
    {
      /* TODO : Message 처리 */
      printf("Recv Config Req Sig \n");

    }

  }

}
