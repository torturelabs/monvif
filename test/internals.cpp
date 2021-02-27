#include <gtest/gtest.h>
#include <mxml.h>

#include "../src/xml.c"

const char *onvifer_GetStreamUri =
	"<v:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\" "
	"xmlns:d=\"http://www.w3.org/2001/XMLSchema\" "
	"xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\" "
	"xmlns:v=\"http://www.w3.org/2003/05/soap-envelope\"><v:Header><Action "
	"mustUnderstand=\"1\" "
	"xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver10/"
	"media/wsdl/GetStreamUri</Action></v:Header><v:Body><GetStreamUri "
	"xmlns=\"http://www.onvif.org/ver10/media/wsdl\"><StreamSetup>\n"
	"   <Stream "
	"xmlns=\"http://www.onvif.org/ver10/schema\">RTP-Unicast</Stream>\n"
	"   <Transport xmlns=\"http://www.onvif.org/ver10/schema\">\n"
	"      <Protocol>RTSP</Protocol>\n"
	"   </Transport>\n"
	"</StreamSetup><ProfileToken>PROFILE_000</ProfileToken></GetStreamUri></"
	"v:Body></v:Envelope>";

const char *onvifer_GetStreamUri_NS =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope\n"
	"xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
	"xmlns:d=\"http://www.w3.org/2001/XMLSchema\"\n"
	"xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"\n"
	"xmlns:v=\"http://www.w3.org/2003/05/soap-envelope\"\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Header\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Action\n"
	"mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\">http://www.onvif.org/ver10/media/wsdl/GetStreamUri</Action></Header><Body\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><GetStreamUri\n"
	"xmlns=\"http://www.onvif.org/ver10/media/wsdl\"><StreamSetup> <Stream\n"
	"xmlns=\"http://www.onvif.org/ver10/schema\">RTP-Unicast</Stream> <Transport\n"
	"xmlns=\"http://www.onvif.org/ver10/schema\"> <Protocol>RTSP</Protocol> </Transport>\n"
	"</StreamSetup><ProfileToken>PROFILE_000</ProfileToken></GetStreamUri></Body></Envelope>\n";

TEST(Internals, ParsingSOAP)
{
	mxml_node_t *doc = parse_soap(onvifer_GetStreamUri);
	const char *parsed = mxmlSaveAllocString(doc, MXML_NO_CALLBACK);
	mxmlDelete(doc);

	ASSERT_STREQ(parsed, onvifer_GetStreamUri_NS);
	std::free((void *)parsed);
}
