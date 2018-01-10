var parser = new DOMParser();
var interestsXmlDoc = parser.parseFromString("<Interests></Interests>", "text/xml");

function findNode(parent, nodeType, nodeValue) {
    var nodes = parent.getElementsByTagName(nodeType);
    for (var i = 0; i < nodes.length; i++) {
        if (nodes[i].nodeType == 1 && nodes[i].attributes['name'].nodeValue == nodeValue) {
            return nodes[i];
        }
    }

    var newElem = interestsXmlDoc.createElement(nodeType);
    newElem.setAttribute("name", nodeValue);
    parent.appendChild(newElem);

    return newElem;
}

function addUser() {
    var formData = document.userForm;

    if (formData.studentName.value == "" || formData.facultyName.value == "" || formData.area.value == "") {
        alert("No field can be empty!!");
        return;
    }

    var parent = findNode(interestsXmlDoc.childNodes[0], "Area", formData.area.value);
    parent = findNode(parent, "Faculty", formData.facultyName.value);

    var newStudent = interestsXmlDoc.createElement("Student");
    var newStudentName = interestsXmlDoc.createTextNode(formData.studentName.value);

    newStudent.appendChild(newStudentName);
    parent.appendChild(newStudent);
}