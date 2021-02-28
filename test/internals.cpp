#include <gtest/gtest.h>
#include <mxml.h>

#include "../src/sha1.c"
#include "../src/soap.c"
#include "../src/tools.c"
#include "../src/xml.c"

typedef struct {
	const char *input_xml;
	const char *norm_xml;
	const char *action;
} soap_tests_t;

const char *onvifer_GetStreamUri =
	"<v:Envelope xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
	"xmlns:d=\"http://www.w3.org/2001/XMLSchema\"\n"
	"xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"\n"
	"xmlns:v=\"http://www.w3.org/2003/05/soap-envelope\">\n"
	"    <v:Header>\n"
	"        <Action mustUnderstand=\"1\"\n"
	"        xmlns=\"http://www.w3.org/2005/08/addressing\">\n"
	"        http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime</Action>\n"
	"    </v:Header>\n"
	"    <v:Body>\n"
	"        <GetSystemDateAndTime xmlns=\"http://www.onvif.org/ver10/device/wsdl\" />\n"
	"    </v:Body>\n"
	"</v:Envelope>\n";

const char *onvifer_GetStreamUri_norm =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope\n"
	"xmlns:i=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
	"xmlns:d=\"http://www.w3.org/2001/XMLSchema\"\n"
	"xmlns:c=\"http://www.w3.org/2003/05/soap-encoding\"\n"
	"xmlns:v=\"http://www.w3.org/2003/05/soap-envelope\"\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <Header\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <Action\n"
	"mustUnderstand=\"1\" xmlns=\"http://www.w3.org/2005/08/addressing\"> http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime</Action>\n"
	"</Header> <Body xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <GetSystemDateAndTime\n"
	"xmlns=\"http://www.onvif.org/ver10/device/wsdl\" /> </Body> </Envelope>\n";

const char *NVR_GetCapabilities =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
	"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\" xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:ter=\"http://www.onvif.org/ver10/error\">\n"
	"  <SOAP-ENV:Header>\n"
	"    <wsse:Security>\n"
	"      <wsse:UsernameToken>\n"
	"        <wsse:Username>admin</wsse:Username>\n"
	"        <wsse:Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">D79qON3XYDxwl0aVjlDPStpaoPk=</wsse:Password>\n"
	"        <wsse:Nonce>NaUwW40j82HBzKtraLWHMi8dBHU=</wsse:Nonce>\n"
	"        <wsu:Created>2018-06-25T00:17:57Z</wsu:Created>\n"
	"      </wsse:UsernameToken>\n"
	"    </wsse:Security>\n"
	"  </SOAP-ENV:Header>\n"
	"  <SOAP-ENV:Body>\n"
	"    <tds:GetCapabilities>\n"
	"      <tds:Category>All</tds:Category>\n"
	"    </tds:GetCapabilities>\n"
	"  </SOAP-ENV:Body>\n"
	"</SOAP-ENV:Envelope>";

const char *NVR_GetCapabilities_norm =
	"<?xml version=\"1.0\" encoding=\"utf-8\"?> <Envelope\n"
	"xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\"\n"
	"xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\"\n"
	"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
	"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
	"xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"\n"
	"xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\"\n"
	"xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\"\n"
	"xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\"\n"
	"xmlns:tt=\"http://www.onvif.org/ver10/schema\"\n"
	"xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\"\n"
	"xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\"\n"
	"xmlns:tmd=\"http://www.onvif.org/ver10/deviceIO/wsdl\"\n"
	"xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\"\n"
	"xmlns:ter=\"http://www.onvif.org/ver10/error\"\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <Header\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <Security\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">\n"
	"<UsernameToken\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">\n"
	"<Username\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">admin</Username>\n"
	"<Password\n"
	"Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\"\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">D79qON3XYDxwl0aVjlDPStpaoPk=</Password>\n"
	"<Nonce\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\">NaUwW40j82HBzKtraLWHMi8dBHU=</Nonce>\n"
	"<Created\n"
	"xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">2018-06-25T00:17:57Z</Created>\n"
	"</UsernameToken> </Security> </Header> <Body\n"
	"xmlns=\"http://www.w3.org/2003/05/soap-envelope\"> <GetCapabilities\n"
	"xmlns=\"http://www.onvif.org/ver10/device/wsdl\"> <Category\n"
	"xmlns=\"http://www.onvif.org/ver10/device/wsdl\">All</Category> </GetCapabilities>\n"
	"</Body> </Envelope>\n";

soap_tests_t tt[] = { soap_tests_t{ .input_xml = onvifer_GetStreamUri,
				    .norm_xml = onvifer_GetStreamUri_norm,
				    .action = "GetSystemDateAndTime" },
		      soap_tests_t{
			      .input_xml = NVR_GetCapabilities,
			      .norm_xml = NVR_GetCapabilities_norm,
			      .action = "GetCapabilities",
		      } };

// TODO: add more samples
// Use https://tomeko.net/online_tools/cpp_text_escape.php?lang=en to convert
// raw XML
TEST(Internals, NormalizeSOAP)
{
	for (const auto &tcase : tt) {
		mxml_node_t *doc = parse_soap(tcase.input_xml);
		const char *action = soap_action(body_element(doc));
		ASSERT_STREQ(action, tcase.action);

		const char *parsed = mxmlSaveAllocString(doc, MXML_NO_CALLBACK);
		mxmlDelete(doc);

		ASSERT_STREQ(parsed, tcase.norm_xml);
		std::free((void *)parsed);
	}
}

TEST(Internals, PasswordDigest)
{
	char digest[30];
	PasswordDigest("NjAzNTFmNDJkYjI2NjUyMmM3OGE=",
		       "2019-08-14T21:30:15.000Z", "", digest);
	ASSERT_STREQ("qL9AM5nU5ag5vU7m04xqf2Vad3U=", digest);
}
