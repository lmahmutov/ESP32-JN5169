int db_open(const char *filename, sqlite3 **db) {
  int rc = sqlite3_open(filename, db);
  if (rc) {
    Serial.printf("Can't open database: %s\n", sqlite3_errmsg(*db));
    return rc;
  } else {
    Serial.printf("Opened database successfully\n");
  }
  return rc;
}

const char* data = "Callback function called";
static int callback(void *data, int argc, char **argv, char **azColName) {
   int i;
   Serial.printf("%s: ", (const char*)data);
   for (i = 0; i<argc; i++){
       Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   Serial.printf("\n");
   return 0;
}

char *zErrMsg = 0;
int db_exec(sqlite3 *db, const char *sql) {
   Serial.println(sql);
   long start = micros();
   int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if (rc != SQLITE_OK) {
       Serial.printf("SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
   } else {
       Serial.printf("Operation done successfully\n");
   }
   Serial.print(F("Time taken:"));
   Serial.println(micros()-start);
   return rc;
}



int sqlite_select_answer(String IEEE)
{
  String sql = "select * from devices where IEEE = '" + IEEE + "'";
  rc = sqlite3_prepare_v2(db, sql.c_str(), 1000, &res, &tail);
  if (rc != SQLITE_OK) {
    String resp = "Failed to fetch data: ";
    Serial.println(resp.c_str());
    return 0;
  }
  rec_count = 0;
  while (sqlite3_step(res) == SQLITE_ROW) {
    rec_count++;
  }
  sqlite3_finalize(res);
  return rec_count;
}

void sqlite_insertnewdev(String IEEE, String devname, String longaddr)
{
  String sql = "INSERT INTO devices (IEEE,name,shortaddr) VALUES('" + IEEE + "','" + devname + "','" + longaddr + "');";
   rc = db_exec(db, sql.c_str());
   if (rc != SQLITE_OK) {
       return;
   }
}

/*
void sqlite_insertnewdev(String IEEE, String devname, String longaddr)
{
  String sql = "INSERT INTO devices (IEEE,name,shortaddr) VALUES('" + IEEE + "','" + devname + "','" + longaddr + "');";
   rc = db_exec(db, sql.c_str());
   if (rc != SQLITE_OK) {
       return;
   }
}
*/
