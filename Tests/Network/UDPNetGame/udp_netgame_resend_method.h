
#pragma once

enum UDPNetGameResendMethod
{
	no_resend,
	resend_if_packet_lost,
	resend_every_packet_until_acked
};
