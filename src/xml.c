#include <assert.h>
#include <stdbool.h>

#include <mxml.h>

#include "xml.h"

#define MAX_LEN 1024

#define XMLNS_PREFIX "xmlns:"
static const char *skip_xmlns(const char *name)
{
	size_t prsz = sizeof(XMLNS_PREFIX) - 1;
	if (!strncmp(name, XMLNS_PREFIX, prsz)) {
		return name + prsz;
	}
	return name;
}

static bool ns_from_parent(mxml_node_t *node, const char *prefix, char *long_ns,
			   size_t len)
{
	assert(node);

	while (node) {
		int i, count;
		const char *name, *value;

		for (i = 0, count = mxmlElementGetAttrCount(node); i < count;
		     i++) {
			value = mxmlElementGetAttrByIndex(node, i, &name);
			if (!strcmp(prefix, skip_xmlns(name))) {
				strncpy(long_ns, value, len - 1);
				return true;
			}
		}
		node = mxmlGetParent(node);
	}
	return false;
}

static const char *name_wo_ns(const char *name)
{
	const char *comma = strchr(name, ':');
	if (comma) {
		return comma + 1;
	}
	return name;
}

static void soap_normalize(mxml_node_t *node, mxml_sax_event_t event,
			   void *data)
{
	mxmlRetain(node);
	if (event != MXML_SAX_ELEMENT_CLOSE)
		return;

	const char *name = mxmlGetElement(node);
	size_t lname = strlen(name);
	if (lname < MAX_LEN) {
		char *nbuf = (char *)alloca(lname + 1);
		strcpy(nbuf, name);

		char *comma = strchr(nbuf, ':');
		if (comma) {
			const char *pure_name = name_wo_ns(nbuf);
			*comma = 0;
			char long_ns[MAX_LEN];
			if (ns_from_parent(node, nbuf, long_ns,
					   sizeof(long_ns))) {
				mxmlElementSetAttr(node, "xmlns", long_ns);
			} else {
				mxmlElementSetAttr(node, "badns", nbuf);
			}
			mxmlSetElement(node, pure_name);
		}
	}
}

static mxml_node_t *parse_soap(const char *xml)
{
	// make input compatible with broken SOAP clients like Android Onvifer
	mxml_node_t *fake_root = NULL;
	if (strncmp("<?xml ", xml, 6)) {
		fake_root = mxmlNewXML("1.0");
	}
	mxml_node_t *tree = mxmlSAXLoadString(
		fake_root, xml, MXML_TEXT_CALLBACK, soap_normalize, NULL);

	return tree;
}

static mxml_node_t *body_element(mxml_node_t *doc)
{
	return mxmlFindElement(doc, doc, "Body", "xmlns",
			       "http://www.w3.org/2003/05/soap-envelope",
			       MXML_DESCEND);
}

static const char *first_child_of(mxml_node_t *tree, mxml_node_t *node)
{
	if (!node)
		return NULL;

	while ((node = mxmlWalkNext(node, tree, MXML_DESCEND)) != NULL) {
		const char *element = mxmlGetElement(node);
		if (!element)
			continue;
		return element;
	}
	return NULL;
}

static const char *soap_action(mxml_node_t *body)
{
	return first_child_of(body, body);
}
