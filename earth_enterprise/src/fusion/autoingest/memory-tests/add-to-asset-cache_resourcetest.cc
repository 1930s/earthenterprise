#include <iomanip>
#include <iostream>
#include <sstream>

#include <mem_usage/raster_project_function_tester.hpp>

#include <autoingest/AssetVersion.h>



class AddToAssetCache_Tester : public opengee::mem_usage::RasterProjectFunctionTester
{
    public:

    // A rather useless count.  Used instead of no operation to look up an
    // asset in the cache.
    int asset_ref_character_count = 0;

    AddToAssetCache_Tester(
        std::string test_name="AddToAssetCache")
    : opengee::mem_usage::RasterProjectFunctionTester(test_name)
    {}

    virtual bool run_test_operation()
    {
        std::string asset_name = get_next_asset_name();

        if (!asset_name_exists(asset_name))
        {
            // Break operation processing:
            return true;
        }

        // Construct an asset reference:
        Asset asset(asset_name);

        asset.CacheAdd();

        // Cause the reference to be resolved by loading the XML:
        asset_ref_character_count += asset->GetRef().length();

        asset.CacheRemove();

        // Keep processing operations:
        return false;
    }

    protected:

    virtual void log_completion_fields(
        int complete_operations_count,
        const struct rusage &start_resources,
        const struct rusage &end_resources)
    {
        opengee::mem_usage::RasterProjectFunctionTester::log_completion_fields(
            complete_operations_count, start_resources, end_resources);
        log_emitter << YAML::Key << "total AssetRef character count" <<
            YAML::Value << asset_ref_character_count;
    }
};

int main(int argc, char** argv)
{
    try
    {
        AddToAssetCache_Tester tester;

        tester.run_test();
    }
    catch (std::system_error &exc)
    {
        std::cerr << exc.code().message() << std::endl;
        throw;
    }

    return 0;
}