#ifndef __DLL_PARSER_H__
#define __DLL_PARSER_H__

#include <QWidget>
#include <vector>

#include <QTreeWidget>
#include <QTableWidget>
#include <QToolbar>

/*
 */
class DLLParser : public QWidget {
	
		Q_OBJECT

	public:
		/*
		*/
		class WidgetToolbar : public QToolBar {

		public:
			WidgetToolbar(QWidget* parent = nullptr);
			~WidgetToolbar();

			void addActions();
			void removeActions();

			// private:
            QAction* m_fileOpen;
		};

		explicit DLLParser(QWidget *parent = nullptr);
		virtual ~DLLParser();

		void setFileName(const QString& filename);
		QString getFileName() const;

		void setFuncName(const QString& funcName);
		QString getFuncName() const;

		void initTableHeader();
		bool analyseExportTable();
		bool analyseHeaderFile();

		void processLine(const QString& buffer);
		void processLine(const QByteArray& buffer);

	private slots:
        void callback_openFile();
		void callback_itemClicked(QTableWidgetItem* item);

	private:
		WidgetToolbar* m_toolbar;

		QTableWidget *m_tree;
		QTreeWidgetItem* m_column;

		QString m_fileName;
		QString m_funcName;
		QByteArray m_fileBuffer;
		QByteArray m_fileHeaderBuffer;

		std::vector<std::string> exportTable;
};

#endif /* __DLL_PARSER_H__ */
