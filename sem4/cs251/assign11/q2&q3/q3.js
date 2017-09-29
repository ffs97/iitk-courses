function prettyXML(xmlText) {
    var indent = 0;

    var lines = xmlText.replace(/<\//g, "\n</").replace(/></g, ">\n<").replace(/>\n/g, ">").replace(/>/g, ">\n").split("\n");
    xmlText = "";
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i];

        var indentStart = (line[0] == '<') && (line[1] != '/');
        var indentEnd = ((line[0] == '<') && (line[1] == '/')) || ((line[line.length - 1] == '>') && (line[line.length - 2] == '/'));

        if (indentEnd) {
            indent--;
            if (indent < 0) indent = 0;
        }

        line = (new Array(8*(indent)).join(" ")) + line;

        if (indentStart) {
            indent++;
        }

        xmlText += line + "\n";
    }

    return xmlText;
}

function showData() {

    console.log(interestsXmlDoc);

    var xmlText = new XMLSerializer().serializeToString(interestsXmlDoc);

    document.getElementById('xml-data').innerText = prettyXML(xmlText);
    document.getElementById('xml-schema').innerText = prettyXML("<xs:schema attributeFormDefault='unqualified' elementFormDefault='qualified' xmlns:xs='http://www.w3.org/2001/XMLSchema'><xs:element name='Interests'><xs:complexType><xs:sequence><xs:element name='Area' maxOccurs='unbounded' minOccurs='0'><xs:complexType><xs:sequence><xs:element name='Faculty'><xs:complexType><xs:sequence><xs:element type='xs:string' name='Student' maxOccurs='unbounded' minOccurs='0'/></xs:sequence><xs:attribute type='xs:string' name='name'/></xs:complexType></xs:element></xs:sequence><xs:attribute type='xs:string' name='name'/></xs:complexType></xs:element></xs:sequence></xs:complexType></xs:element></xs:schema>");

}

function myFunction(xml) {
}