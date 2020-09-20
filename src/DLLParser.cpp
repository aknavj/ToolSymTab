#pragma comment(lib,"Dbghelp.lib")

#include "DLLParser.h"
#include "Console.h"
#include "utils/Base.h"

#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>

#include <Windows.h>
#include <DbgHelp.h> // ImageRvaToVa

#include <set>

/*
 */
DLLParser::DLLParser(QWidget *parent) : QWidget(parent) {

	m_toolbar = new ::DLLParser::WidgetToolbar(this);
    connect(m_toolbar->m_fileOpen, SIGNAL(triggered(bool)), this, SLOT(callback_openFile()));

	m_tree = new QTableWidget;
	m_tree->setColumnCount(4);

	QMainWindow* inner_widget = new QMainWindow(0);
	inner_widget->addToolBar(Qt::TopToolBarArea, m_toolbar);
	inner_widget->setCentralWidget(m_tree);
	inner_widget->setParent(this);

	QVBoxLayout* m_mainlayout = new QVBoxLayout;
	m_mainlayout->setSpacing(0);
	m_mainlayout->setContentsMargins(0, 0, 0, 0);
	m_mainlayout->addWidget(inner_widget);
	this->setLayout(m_mainlayout);

	initTableHeader();
}

DLLParser::~DLLParser() {
	exportTable.clear();
}

/*
 */
void DLLParser::initTableHeader() {

	QStringList labels;
	labels.append(QStringLiteral("Ordinal"));
	labels.append(QStringLiteral("RVA"));
	labels.append(QStringLiteral("Name Ordinal"));
	labels.append(QStringLiteral("Function Name"));
	labels.append(QStringLiteral("Header Linkage"));

	m_tree->setHorizontalHeaderLabels(labels);
	m_tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_tree->setSelectionBehavior(QAbstractItemView::SelectRows);
}

/*
 */
void DLLParser::callback_openFile() {

	QString fp = QFileDialog::getOpenFileName(this, tr("Open Dynamic Load Library"), "", tr("DLL (*.dll)"));

	if (!fp.isEmpty()) {
		//ui->FILEPATH_EDIT->setText(fp);

		QFileInfo fi(fp);
		m_fileName = fi.fileName();
		QFile file(fp);

		if (file.open(QIODevice::ReadOnly)) {
			m_fileBuffer = file.readAll();
			analyseExportTable();
			file.close();
		}
	}
}

/*
 */
void DLLParser::callback_itemClicked(QTableWidgetItem* item) {
	Console::message("DLLParser::callback_itemClicked(): " + QString("%1").arg(item->row()));
	if (m_tree->item(item->row(), 3)->text() != "-") {
		m_funcName = m_tree->item(item->row(), 3)->text();
		Console::message("\tm_funcName: " + QString("%1").arg(m_funcName));
	}
}

/*
 */
bool DLLParser::analyseExportTable() {

	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)m_fileBuffer.data();
	PIMAGE_NT_HEADERS32 pNTHeader = (PIMAGE_NT_HEADERS32)(pDOSHeader->e_lfanew + m_fileBuffer.data());
	PIMAGE_NT_HEADERS64 pNTHeader64 = (PIMAGE_NT_HEADERS64)(pDOSHeader->e_lfanew + m_fileBuffer.data());

	PIMAGE_DATA_DIRECTORY pExportTable;
	DWORD dwExportAddr;
	PIMAGE_EXPORT_DIRECTORY pExportAddr;
	DWORD dwExportNameFOA;
	DWORD dwExportAddressOfNameOrdinalsFOA;
	DWORD dwExportAddressOfFunctionsFOA;

	////////////////////////
	// 64 bit based library
	////////////////////////
	if (pNTHeader->FileHeader.Machine != 0x014c) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("64-bit based platform!"), QMessageBox::Yes);

		pExportTable = pNTHeader64->OptionalHeader.DataDirectory;
		if (pExportTable->VirtualAddress == NULL) {
			QMessageBox::warning(NULL, "warning", QStringLiteral("PE File doesnt have Export Table!"), QMessageBox::Yes);
			return false;
		}

		dwExportAddr = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader64, (PVOID)0, pExportTable->VirtualAddress, NULL));
		pExportAddr = (PIMAGE_EXPORT_DIRECTORY)(m_fileBuffer.data() + dwExportAddr);

		m_tree->setRowCount(pExportAddr->NumberOfFunctions);

		dwExportNameFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfNames, NULL));
		dwExportAddressOfNameOrdinalsFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfNameOrdinals, NULL));
		dwExportAddressOfFunctionsFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfFunctions, NULL));

		for (int i = 0; i < pExportAddr->NumberOfFunctions; i++) {
			DWORD* pFuncRVA64 = (DWORD*)(m_fileBuffer.data() + dwExportAddressOfFunctionsFOA + i * sizeof(DWORD));
			if (*pFuncRVA64 != NULL) {
				m_tree->setItem(i, 0, new QTableWidgetItem(tr("%1").arg(i + 1, 8, 16, QLatin1Char('0')).toUpper()));
				m_tree->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(*pFuncRVA64, 8, 16, QLatin1Char('0')).toUpper()));
				m_tree->setItem(i, 2, new QTableWidgetItem("-"));
				m_tree->setItem(i, 3, new QTableWidgetItem("-"));
				m_tree->setItem(i, 4, new QTableWidgetItem("-"));
			} else {
				m_tree->setItem(i, 0, new QTableWidgetItem("-"));
				m_tree->setItem(i, 1, new QTableWidgetItem("-"));
				m_tree->setItem(i, 2, new QTableWidgetItem("-"));
				m_tree->setItem(i, 3, new QTableWidgetItem("-"));
				m_tree->setItem(i, 4, new QTableWidgetItem("-"));
			}
		}

		for (DWORD n = 0; n < pExportAddr->NumberOfNames; n++) {
			DWORD* pFuncNameRVA64 = (DWORD*)(m_fileBuffer.data() + dwExportNameFOA + n * sizeof(DWORD));
			DWORD dwFuncNameFOA64 = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, *pFuncNameRVA64, NULL));
			WORD* AddressOfNameOrdinals64 = (WORD*)(m_fileBuffer.data() + dwExportAddressOfNameOrdinalsFOA + n * sizeof(WORD));
			
			m_tree->setItem(*AddressOfNameOrdinals64, 3, new QTableWidgetItem(QString((char*)(m_fileBuffer.data() + dwFuncNameFOA64))));
			m_tree->setItem(*AddressOfNameOrdinals64, 2, new QTableWidgetItem(tr("%1").arg(*AddressOfNameOrdinals64, 4, 16, QLatin1Char('0')).toUpper()));
		}

		return true;
	}

	////////////////////////
	// 32bit based library
	////////////////////////
	pExportTable = pNTHeader->OptionalHeader.DataDirectory;
	if (pExportTable->VirtualAddress == NULL) {
		QMessageBox::warning(NULL, "warning", QStringLiteral("PE File doesnt have Export Table!"), QMessageBox::Yes);
		//ui->FILEPATH_EDIT->clear();
		return false;
	}

	dwExportAddr = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportTable->VirtualAddress, NULL));
	pExportAddr = (PIMAGE_EXPORT_DIRECTORY)(m_fileBuffer.data() + dwExportAddr);
	m_tree->setRowCount(pExportAddr->NumberOfFunctions);

	dwExportNameFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfNames, NULL));
	dwExportAddressOfNameOrdinalsFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfNameOrdinals, NULL));
	dwExportAddressOfFunctionsFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, pExportAddr->AddressOfFunctions, NULL));

	for (int i = 0; i < pExportAddr->NumberOfFunctions; i++) {
		DWORD* pFuncRVA = (DWORD*)(m_fileBuffer.data() + dwExportAddressOfFunctionsFOA + i * sizeof(DWORD));
		if (*pFuncRVA != NULL) {
			m_tree->setItem(i, 0, new QTableWidgetItem(tr("%1").arg(i + 1, 8, 16, QLatin1Char('0')).toUpper()));
			m_tree->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(*pFuncRVA, 8, 16, QLatin1Char('0')).toUpper()));
			m_tree->setItem(i, 2, new QTableWidgetItem("-"));
			m_tree->setItem(i, 3, new QTableWidgetItem("-"));
			m_tree->setItem(i, 4, new QTableWidgetItem("-"));
		} else {
			m_tree->setItem(i, 0, new QTableWidgetItem("-"));
			m_tree->setItem(i, 1, new QTableWidgetItem("-"));
			m_tree->setItem(i, 2, new QTableWidgetItem("-"));
			m_tree->setItem(i, 3, new QTableWidgetItem("-"));
			m_tree->setItem(i, 4, new QTableWidgetItem("-"));
		}
	}

	for (DWORD n = 0; n < pExportAddr->NumberOfNames; n++) {
		DWORD* pFuncNameRVA = (DWORD*)(m_fileBuffer.data() + dwExportNameFOA + n * sizeof(DWORD));
		DWORD dwFuncNameFOA = (DWORD)(ImageRvaToVa((PIMAGE_NT_HEADERS)pNTHeader, (PVOID)0, *pFuncNameRVA, NULL));	
		WORD* AddressOfNameOrdinals = (WORD*)(m_fileBuffer.data() + dwExportAddressOfNameOrdinalsFOA + n * sizeof(WORD));
		
		m_tree->setItem(*AddressOfNameOrdinals, 2, new QTableWidgetItem(tr("%1").arg(*AddressOfNameOrdinals, 4, 16, QLatin1Char('0')).toUpper()));
		m_tree->setItem(*AddressOfNameOrdinals, 3, new QTableWidgetItem(QString((char*)(m_fileBuffer.data() + dwFuncNameFOA))));
	}

	return true;
}

/*
 */
bool DLLParser::analyseHeaderFile() {
	return true;
}

void DLLParser::processLine(const QString& buffer) {
	Console::message(buffer);
}

void DLLParser::processLine(const QByteArray& buffer) {
	
	int first = 0;
	QByteArray newBuffer;

	// remove tabs and spaces
	for (int i = 0; i < buffer.size(); i++) {
		char c = buffer[i];
		if (c != ' ' || c != '\t' || c != '\0') first = true;
		if (first) newBuffer += c;
	}

	//////////////////////////////
	// quick line first pass
	//////////////////////////////
	QByteArray buf2;
	char c = newBuffer[0];
	switch (c) {
		case '\0': break;
		case '\n': break;
		case '#': break;
		case '*': break;
		case '/': break;
		case ' ': if (newBuffer[1] == '*') break;
		default: Console::message(newBuffer); break;
	}

	// (typ) (stdcall__) (Nazev Funkce)(argumenty,...)
	
}

/******************************************************************************\
*
* WidgetConsoleToolbar
*
\******************************************************************************/

/*
 */
DLLParser::WidgetToolbar::WidgetToolbar(QWidget* parent) : QToolBar(parent) {

	// add actions
	addActions();
}

/*
 */
DLLParser::WidgetToolbar::~WidgetToolbar() {

	// remove actions
	removeActions();
}

/*
 */
void DLLParser::WidgetToolbar::addActions() {
    m_fileOpen = new QAction(tr("Load DLL"));
    this->addAction(m_fileOpen);
}

/*
 */
void DLLParser::WidgetToolbar::removeActions() {
    delete m_fileOpen;
}
