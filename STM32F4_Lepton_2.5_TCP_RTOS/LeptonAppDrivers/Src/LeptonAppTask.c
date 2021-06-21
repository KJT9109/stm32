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
  lepton_id = osEventFlagsNew(NULL);

  if (lepton_id == NULL)
  {
    /* TODO : Error 메시지 및 처리 */
  }

  /* TODO : Lepton Init 구현 */

  for (;;)
  {
    /* TODO :  Check Wait Video Frame Request Signal */
    if (osEventFlagsWait(lepton_id, VIDEO_REQ_SIGNAL, osFlagsWaitAny, 1000) < 0)
    {
      /* TODO : Error 처리 */
    }
    else
    {
      /* TODO : Start Video Frame Capture and send Mail Queue */
    }

    /* TODO : Wait Lepton Configuration Request Signal */

    if (osEventFlagsWait(lepton_id, LEPTON_CONFIG_SIGNAL, osFlagsWaitAny, 1000) < 0)
    {
      /* TODO : Error 처리 */
    }
    else
    {
      /* TODO : Set Lepton Configuration and Reply */
    }
  }

}
