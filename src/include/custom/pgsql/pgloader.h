/*
 * Legal Notice
 *
 * This document and associated source code (the "Work") is a part of a
 * benchmark specification maintained by the TPC.
 *
 * The TPC reserves all right, title, and interest to the Work as provided
 * under U.S. and international laws, including without limitation all patent
 * and trademark rights therein.
 *
 * No Warranty
 *
 * 1.1 TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THE INFORMATION
 *     CONTAINED HEREIN IS PROVIDED "AS IS" AND WITH ALL FAULTS, AND THE
 *     AUTHORS AND DEVELOPERS OF THE WORK HEREBY DISCLAIM ALL OTHER
 *     WARRANTIES AND CONDITIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
 *     INCLUDING, BUT NOT LIMITED TO, ANY (IF ANY) IMPLIED WARRANTIES,
 *     DUTIES OR CONDITIONS OF MERCHANTABILITY, OF FITNESS FOR A PARTICULAR
 *     PURPOSE, OF ACCURACY OR COMPLETENESS OF RESPONSES, OF RESULTS, OF
 *     WORKMANLIKE EFFORT, OF LACK OF VIRUSES, AND OF LACK OF NEGLIGENCE.
 *     ALSO, THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
 *     QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR NON-INFRINGEMENT
 *     WITH REGARD TO THE WORK.
 * 1.2 IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THE WORK BE LIABLE TO
 *     ANY OTHER PARTY FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO THE
 *     COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST PROFITS, LOSS
 *     OF USE, LOSS OF DATA, OR ANY INCIDENTAL, CONSEQUENTIAL, DIRECT,
 *     INDIRECT, OR SPECIAL DAMAGES WHETHER UNDER CONTRACT, TORT, WARRANTY,
 *     OR OTHERWISE, ARISING IN ANY WAY OUT OF THIS OR ANY OTHER AGREEMENT
 *     RELATING TO THE WORK, WHETHER OR NOT SUCH AUTHOR OR DEVELOPER HAD
 *     ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * Contributors
 * - 2006 Rilson Nascimento
 * - 2010 Mark Wong <markwkm@postgresql.org>
 */

//
// Class representing PostgreSQL database loader.
//

#ifndef PG_LOADER_H
#define PG_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

namespace TPCE
{
const int iDateTimeFmt = 11;
const int iConnectStrLen = 256;

//
// PGSQLLoader class.
//
template <typename T> class CPGSQLLoader: public CBaseLoader<T>
{
private:
	void Copy();

protected:
	FILE *p;

	char m_szConnectStr[iConnectStrLen + 1];
	char m_szTable[iMaxPath + 1]; // name of the table being loaded

public:
	typedef const T *PT; // pointer to the table row

	CPGSQLLoader(const char *szConnectStr, const char *szTable);
	~CPGSQLLoader(void);

	// resets to clean state; needed after FinishLoad to continue loading
	void Init();

	void Commit(); // commit rows sent so far
	void FinishLoad(); // finish load
	void Connect(); // connect to PostgreSQL

	// disconnect - should not throw any exceptions (to put into the
	// destructor)
	void Disconnect();

	void
	WriteNextRecord(const T &next_record)
	{
		printf("pgloader - const ref\n");
	};
};

//
// The constructor.
//
template <typename T>
CPGSQLLoader<T>::CPGSQLLoader(const char *szConnectStr, const char *szTable)
{
	// FIXME: This may truncate if the szConnectStr is actually close to
	// iConnectStrLen.
	snprintf(m_szConnectStr, iConnectStrLen,
			"psql --quiet --output=psql-%d-%s.log %s", getpid(), szTable,
			szConnectStr);

	strncpy(m_szTable, szTable, iMaxPath);
}

//
// Destructor closes the connection.
//
template <typename T> CPGSQLLoader<T>::~CPGSQLLoader()
{
	Disconnect();
}

//
// Reset state e.g. close the connection, bind columns again, and reopen.
// Needed after Commit() to continue loading.
//
template <typename T>
void
CPGSQLLoader<T>::Init()
{
	Connect();
}

template <typename T>
void
CPGSQLLoader<T>::Connect()
{
	// Open a pipe to psql.
	p = popen(m_szConnectStr, "w");
	if (p == NULL) {
		cout << "error using psql" << endl;
		exit(1);
	}
	// FIXME: Have blind faith that psql connected ok.
	while (fgetc(p) != EOF)
		;

	Copy();
}

template <typename T>
void
CPGSQLLoader<T>::Copy()
{
	fprintf(p, "BEGIN;\n");
	while (fgetc(p) != EOF)
		;

	fprintf(p, "COPY %s FROM STDIN WITH (DELIMITER '|', NULL '');\n",
			m_szTable);
	// FIXME: Have blind faith that COPY started correctly.
	while (fgetc(p) != EOF)
		;
}

//
// Commit sent rows. This needs to be called every so often to avoid row-level
// lock accumulation.
//
template <typename T>
void
CPGSQLLoader<T>::Commit()
{
	FinishLoad();
	Copy();
}

//
// Commit sent rows. This needs to be called after the last row has been sent
// and before the object is destructed. Otherwise all rows will be discarded
// since this is in a transaction.
//
template <typename T>
void
CPGSQLLoader<T>::FinishLoad()
{
	// End of the COPY.
	fprintf(p, "\\.\n");
	// FIXME: Have blind faith that COPY was successful.
	while (fgetc(p) != EOF)
		;

	// COMMIT the COPY.
	fprintf(p, "COMMIT;\n");
	// FIXME: Have blind faith that COMMIT was successful.
	while (fgetc(p) != EOF)
		;
}

//
// Disconnect from the server. Should not throw any exceptions.
//
template <typename T>
void
CPGSQLLoader<T>::Disconnect()
{
	if (p != NULL) {
		pclose(p);
	}
}

} // namespace TPCE

#endif // PG_LOADER_H
